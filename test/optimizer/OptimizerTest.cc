// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "gtest/gtest.h"
#include "imlab/Database.h"
#include "imlab/algebra/InnerJoin.h"
#include "imlab/algebra/Operator.h"
#include "imlab/algebra/Print.h"
#include "imlab/algebra/Select.h"
#include "imlab/runtime/RuntimeException.h"
#include "imlab/semana/SemanticAnalysis.h"
#include "imlab/statement/QueryStatement.h"
#include "imlab/statement/Statement.h"
#include <array>
#include <sstream>
#include <imlab/algebra/TableScan.h>
//---------------------------------------------------------------------------
using namespace imlab;
//---------------------------------------------------------------------------
namespace test {
//---------------------------------------------------------------------------
std::unique_ptr<statement::Statement> getStatement(imlab::Database& db, std::string query) {
    std::cout << "running: " << query << std::endl;
    std::stringstream s;
    s << query; // << ";";
    parser::ParseContext pc;
    auto r = pc.Parse(s);
    std::vector<std::unique_ptr<statement::Statement>> statements;
    statements.reserve(r.size());
    semana::SemanticAnalysis semana;
    for (auto& ast : r) {
        try {
            auto statement = semana.buildStatement(*ast, db);
            return statement;
        } catch (imlab::RuntimeException& e) {
            std::cerr << e.getMessage();
            assert(false);
        } catch (std::exception& e) {
            throw e;
        }
    }
    return nullptr;
}


void printOperatorTree(const algebra::Operator* op, int depth = 0) {
    if (!op) return;

    std::string indent(depth * 2, ' ');

    if (op->getOperatorType() == algebra::Operator::OperatorType::TableScan) {
        std::cout << indent << "Operator: " << op->getOperatorTypeName() << " " << static_cast<const algebra::TableScan*>(op)->getTableName() << "\n";
    } else {
        std::cout << indent << "Operator: " << op->getOperatorTypeName() << "\n";
    }

    if (auto selectOp = dynamic_cast<const algebra::Select*>(op)) {
        std::cout << indent << "  Condition: " << selectOp->getExpression()->toString() << "\n";
    } else if (auto innerJoinOp = dynamic_cast<const algebra::InnerJoin*>(op)) {
        std::cout << indent << "  Join Conditions: " << innerJoinOp->getConditionsAsString() << "\n";
    }

    if (auto unaryOp = dynamic_cast<const algebra::UnaryOperator*>(op)) {
        printOperatorTree(unaryOp->getInput(), depth + 1);
    } else if (auto binaryOp = dynamic_cast<const algebra::BinaryOperator*>(op)) {
        std::cout << indent << "  Left:\n";
        printOperatorTree(binaryOp->getLeft(), depth + 1);
        std::cout << indent << "  Right:\n";
        printOperatorTree(binaryOp->getRight(), depth + 1);
    }
}


//---------------------------------------------------------------------------
TEST(OptimizerTest, PushDownSelectionAndBuildJoins) {
    imlab::Database db;
    auto create1 = getStatement(db, "create table foo (a integer not null);");
    create1->run(db);
    auto create2 = getStatement(db, "create table bar (b integer not null);");
    create2->run(db);

    auto stmt = getStatement(db, "select * from foo f, bar b where f.a = 1234 and f.a = b.b;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), imlab::statement::Statement::Type::QueryStatementType);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select1 = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select1->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select2 = static_cast<const imlab::algebra::Select*>(select1->getInput());
        ASSERT_EQ(select2->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(select2->getInput());
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }

    // now optimize the statement
    stmt->optimize(imlab::OptimizerPass::PredicatePushdown);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(print->getInput());
        auto opType = innerJoin->getLeft()->getOperatorType();
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }
}


// -------------


TEST(OptimizerTest, SimplePredicatePushdown) {
    imlab::Database db;
    auto create = getStatement(db, "create table simple (x integer not null);");
    create->run(db);

    db.insertRow("simple", {imlab::Value(imlab::Integer(1))});
    db.insertRow("simple", {imlab::Value(imlab::Integer(2))});
    db.insertRow("simple", {imlab::Value(imlab::Integer(3))});

    auto stmt = getStatement(db, "select * from simple s where s.x = 2;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), imlab::statement::Statement::Type::QueryStatementType);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }

    stmt->optimize(imlab::OptimizerPass::PredicatePushdown);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        // select should remain after optimization
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }
}

TEST(OptimizerTest, JoinPredicatePushdown) {
    imlab::Database db;
    auto create1 = getStatement(db, "create table t1 (a integer not null);");
    create1->run(db);
    auto create2 = getStatement(db, "create table t2 (b integer not null);");
    create2->run(db);

    db.insertRow("t1", {imlab::Value(imlab::Integer(10))});
    db.insertRow("t1", {imlab::Value(imlab::Integer(20))});
    db.insertRow("t2", {imlab::Value(imlab::Integer(10))});
    db.insertRow("t2", {imlab::Value(imlab::Integer(30))});

    auto stmt = getStatement(db, "select * from t1, t2 where t1.a = t2.b and t1.a = 20;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), imlab::statement::Statement::Type::QueryStatementType);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select1 = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select1->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select2 = static_cast<const imlab::algebra::Select*>(select1->getInput());
        ASSERT_EQ(select2->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(select2->getInput());
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }

    stmt->optimize(imlab::OptimizerPass::PredicatePushdown);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(print->getInput());
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }
}

TEST(OptimizerTest, TwoSelectNoJoin) {
    imlab::Database db;
    auto create = getStatement(db, "create table foo (a integer not null);");
    create->run(db);
    db.insertRow("foo", {imlab::Value(imlab::Integer(50))});
    db.insertRow("foo", {imlab::Value(imlab::Integer(123))});
    db.insertRow("foo", {imlab::Value(imlab::Integer(200))});
    auto stmt = getStatement(db, "select * from foo f where f.a = 123 and f.a = 200;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), imlab::statement::Statement::Type::QueryStatementType);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select1 = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select1->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select2 = static_cast<const imlab::algebra::Select*>(select1->getInput());
        ASSERT_EQ(select2->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
        printOperatorTree(tree);
    }
    stmt->optimize(imlab::OptimizerPass::PredicatePushdown);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();

        printOperatorTree(tree);

        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }
}

TEST(OptimizerTest, OneSelectOneInnerJoin) {
    imlab::Database db;
    auto createFoo = getStatement(db, "create table foo (a integer not null);");
    createFoo->run(db);
    auto createBar = getStatement(db, "create table bar (b integer not null);");
    createBar->run(db);
    db.insertRow("foo", {imlab::Value(imlab::Integer(10))});
    db.insertRow("foo", {imlab::Value(imlab::Integer(20))});
    db.insertRow("bar", {imlab::Value(imlab::Integer(10))});
    db.insertRow("bar", {imlab::Value(imlab::Integer(30))});
    auto stmt = getStatement(db, "select * from foo f, bar b where f.a = 20 and f.a = b.b;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), imlab::statement::Statement::Type::QueryStatementType);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select1 = static_cast<const imlab::algebra::Select*>(print->getInput());
        ASSERT_EQ(select1->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        auto select2 = static_cast<const imlab::algebra::Select*>(select1->getInput());
        ASSERT_EQ(select2->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(select2->getInput());
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }


    stmt->optimize(imlab::OptimizerPass::PredicatePushdown);
    {
        auto queryStmt = static_cast<imlab::statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();
        ASSERT_EQ(tree->getOperatorType(), imlab::algebra::Operator::OperatorType::Print);
        auto print = static_cast<const imlab::algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), imlab::algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin = static_cast<const imlab::algebra::InnerJoin*>(print->getInput());
        ASSERT_EQ(innerJoin->getLeft()->getOperatorType(), imlab::algebra::Operator::OperatorType::Select);
        ASSERT_EQ(innerJoin->getRight()->getOperatorType(), imlab::algebra::Operator::OperatorType::TableScan);
    }
}


TEST(OptimizerTest, ThreeTablesTwoInnerJoins) {

    printf("ThreeTablesTwoInnerJoins");
    Database db;

    auto createFoo = getStatement(db, "create table A (a integer not null);");
    createFoo->run(db);
    auto createBar = getStatement(db, "create table B (b integer not null);");
    createBar->run(db);
    auto createBaz = getStatement(db, "create table C (c integer not null);");
    createBaz->run(db);

    db.insertRow("A", {Value(Integer(10))});
    db.insertRow("A", {Value(Integer(20))});
    db.insertRow("B", {Value(Integer(20))});
    db.insertRow("B", {Value(Integer(30))});
    db.insertRow("C", {Value(Integer(20))});
    db.insertRow("C", {Value(Integer(40))});

    auto stmt = getStatement(db, "select * from A a, B b, C c where a.a = 20 and a.a = b.b and b.b = c.c;");
    ASSERT_TRUE(stmt);
    ASSERT_EQ(stmt->getType(), statement::Statement::Type::QueryStatementType);

    {
        auto queryStmt = static_cast<statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();

        printOperatorTree(tree);

        ASSERT_EQ(tree->getOperatorType(), algebra::Operator::OperatorType::Print);
        auto print = static_cast<const algebra::Print*>(tree);

        ASSERT_EQ(print->getInput()->getOperatorType(), algebra::Operator::OperatorType::Select);
        auto select1 = static_cast<const algebra::Select*>(print->getInput());

        ASSERT_EQ(select1->getInput()->getOperatorType(), algebra::Operator::OperatorType::Select);
        auto select2 = static_cast<const algebra::Select*>(select1->getInput());

        ASSERT_EQ(select2->getInput()->getOperatorType(), algebra::Operator::OperatorType::Select);
        auto select3 = static_cast<const algebra::Select*>(select2->getInput());

        ASSERT_EQ(select3->getInput()->getOperatorType(), algebra::Operator::OperatorType::InnerJoin);
        auto innerJoin1 = static_cast<const algebra::InnerJoin*>(select3->getInput());

        ASSERT_EQ(innerJoin1->getLeft()->getOperatorType(), algebra::Operator::OperatorType::InnerJoin);
        ASSERT_EQ(innerJoin1->getRight()->getOperatorType(), algebra::Operator::OperatorType::TableScan);

        auto innerJoin2 = static_cast<const algebra::InnerJoin*>(innerJoin1->getLeft());
        ASSERT_EQ(innerJoin2->getLeft()->getOperatorType(), algebra::Operator::OperatorType::TableScan);
        ASSERT_EQ(innerJoin2->getRight()->getOperatorType(), algebra::Operator::OperatorType::TableScan);
    }

    stmt->optimize(OptimizerPass::PredicatePushdown);


    {
        auto queryStmt = static_cast<statement::QueryStatement*>(stmt.get());
        ASSERT_TRUE(queryStmt->getTree());
        auto tree = queryStmt->getTree();

        printOperatorTree(tree);

        ASSERT_EQ(tree->getOperatorType(), algebra::Operator::OperatorType::Print);

        auto print = static_cast<const algebra::Print*>(tree);
        ASSERT_EQ(print->getInput()->getOperatorType(), algebra::Operator::OperatorType::InnerJoin);

        auto innerJoin1 = static_cast<const algebra::InnerJoin*>(print->getInput());
        ASSERT_EQ(innerJoin1->getLeft()->getOperatorType(), algebra::Operator::OperatorType::InnerJoin);
        ASSERT_EQ(innerJoin1->getRight()->getOperatorType(), algebra::Operator::OperatorType::TableScan);

        auto innerJoin2 = static_cast<const algebra::InnerJoin*>(innerJoin1->getLeft());
        ASSERT_EQ(innerJoin2->getLeft()->getOperatorType(), algebra::Operator::OperatorType::Select);
        ASSERT_EQ(innerJoin2->getRight()->getOperatorType(), algebra::Operator::OperatorType::TableScan);

        auto select = static_cast<const algebra::Select*>(innerJoin2->getLeft());
        ASSERT_EQ(select->getInput()->getOperatorType(), algebra::Operator::OperatorType::TableScan);
    }
}



//---------------------------------------------------------------------------
} // namespace test
//---------------------------------------------------------------------------