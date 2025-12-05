// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/semana/SemanticAnalysis.h"
#include "imlab/Database.h"
#include "imlab/algebra/InnerJoin.h"
#include "imlab/algebra/Print.h"
#include "imlab/algebra/Select.h"
#include "imlab/algebra/SubqueryScan.h"
#include "imlab/algebra/TableScan.h"
#include "imlab/parser/AST.h"
#include "imlab/parser/ParseContext.h"
#include "imlab/runtime/RuntimeException.h"
#include "imlab/statement/CopyTableStatement.h"
#include "imlab/statement/CreateTableStatement.h"
#include "imlab/statement/QueryStatement.h"
#include "imlab/statement/Statement.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>
#include <imlab/algebra/BinaryExpression.h>
#include <imlab/algebra/IURef.h>

using Type = imlab::Type;
using ParseContext = imlab::parser::ParseContext;
using SemanticAnalysis = imlab::semana::SemanticAnalysis;
using Database = imlab::Database;

namespace test {

namespace {
void writeToFile(const std::string& fileName, const std::string& content) {
    std::ofstream f(fileName);
    f << content;
}
} // namespace

TEST(SemanaTest, SchemaParseEmptyTable) {
    // parse the statement
    std::istringstream in("create table foo ();");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    Database db;
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::CreateTableStatementType);
    auto stmtPtr = static_cast<imlab::statement::CreateTableStatement*>(statement.get());
    auto& table = stmtPtr->getTable();
    ASSERT_EQ(table.id, "foo");
    ASSERT_TRUE(table.columns.empty());
    ASSERT_TRUE(table.primary_key.empty());
}

TEST(SemanaTest, CopyWithoutDelimiterError) {
    Database db;

    std::istringstream in(R"SQLSCHEMA(
        copy foo from 'bar/foo';
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    ASSERT_THROW(s.buildStatement(*parseTrees[0], db), imlab::RuntimeException);
}

TEST(SemanaTest, CopyWithoutDelimiter) {
    // generate to file imlabtemp
    writeToFile("imlabtemp", "1\n");

    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo (a integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
        SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    std::istringstream in(R"SQLSCHEMA(
        copy foo from 'imlabtemp';
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::CopyTableStatementType);
    auto stmtPtr = static_cast<imlab::statement::CopyTableStatement*>(statement.get());
    EXPECT_EQ(stmtPtr->getTable(), "foo");
    EXPECT_EQ(stmtPtr->getFile(), "imlabtemp");
    EXPECT_EQ(stmtPtr->getDelimiter(), "|");
    ASSERT_TRUE(stmtPtr);
    stmtPtr->run(db);

    // check for column a in table foo that the value 1 was successfully inserted

    const auto& fooTable = db.getTable("foo");
    const auto& fooTableRows = fooTable.getRows();
    const auto& colIndices = fooTable.getColumnIndices();
    ASSERT_TRUE(colIndices.contains("a"));
    ASSERT_EQ(colIndices.at("a"), 0);
    ASSERT_EQ(fooTableRows.size(), 1);
    ASSERT_EQ(fooTableRows[0].size(), 1);
    ASSERT_EQ(fooTableRows[0][colIndices.at("a")].getInteger(), imlab::Integer(1));
}

TEST(SemanaTest, QueryTableNotKnown) {
    Database db;

    std::istringstream in(R"SQLSCHEMA(
        select * from foo;
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    ASSERT_THROW(s.buildStatement(*parseTrees[0], db), imlab::RuntimeException);
}

TEST(SemanaTest, QueryTableScan) {
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo (a integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
            SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    std::istringstream in(R"SQLSCHEMA(
        select * from foo;
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
    auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
    ASSERT_TRUE(stmtPtr->getTree());
    // the input operator should be a Print operator
    auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
    ASSERT_TRUE(printPtr);
    // under the Print operator is a TableScan
    auto tablePtr = dynamic_cast<const imlab::algebra::TableScan*>(printPtr->getInput());
    ASSERT_TRUE(tablePtr);
}

TEST(SemanaTest, QueryTableScanWithOutput) {
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo (a integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
            SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    std::istringstream in(R"SQLSCHEMA(
        select a from foo;
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
    auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
    ASSERT_TRUE(stmtPtr->getTree());
    // the input operator should be a Print operator
    auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
    ASSERT_TRUE(printPtr);
    // under the Print operator is a TableScan
    auto tablePtr = dynamic_cast<const imlab::algebra::TableScan*>(printPtr->getInput());
    ASSERT_TRUE(tablePtr);
}

TEST(SemanaTest, QueryTableScanSelection) {
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo (a integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
            SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    std::istringstream in(R"SQLSCHEMA(
        select * from foo where a = 1234;
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
    auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
    ASSERT_TRUE(stmtPtr->getTree());
    // the input operator should be a Print operator
    auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
    ASSERT_TRUE(printPtr);
    // under the Print operator is a Select
    auto selection = dynamic_cast<const imlab::algebra::Select*>(printPtr->getInput());
    ASSERT_TRUE(selection);
    // under the Select operator is a TableScan
    auto tableScan = dynamic_cast<const imlab::algebra::TableScan*>(selection->getInput());
    ASSERT_TRUE(tableScan);
}

TEST(SemanaTest, QueryTableScanSelectionColumnNotKnown) {
    printf("test0");
    fflush(stdout);
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        printf("test1");
        fflush(stdout);
        std::istringstream in("create table foo (a integer not null, b integer not null);");
        ParseContext spc;
        printf("test2");
        fflush(stdout);
        auto parseTrees = spc.Parse(in);
        printf("Number of parse trees: %zu\n", parseTrees.size());
        fflush(stdout);
        ASSERT_TRUE(!parseTrees.empty());
        ASSERT_TRUE(parseTrees[0] != nullptr);
        SemanticAnalysis s;
        printf("Building statement...\n");
        fflush(stdout);
        auto statement = s.buildStatement(*parseTrees[0], db);
        printf("Statement built successfully.\n");
        fflush(stdout);
        ASSERT_TRUE(statement);
        printf("Running statement...\n");
        fflush(stdout);
        statement->run(db);
        printf("Statement executed successfully.\n");
        fflush(stdout);
    }
    printf("test3");

    std::istringstream in(R"SQLSCHEMA(
        select * from foo where c = 1234 and d = 2345;
    )SQLSCHEMA");
    ParseContext spc;
    printf("Parsing second query...\n");
    fflush(stdout);
    auto parseTrees = spc.Parse(in);
    printf("Parsing second query...\n");
    fflush(stdout);
    ASSERT_TRUE(!parseTrees.empty());

    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    // should fail as c and d are not known
    ASSERT_THROW((s.buildStatement(*parseTrees[0], db)), imlab::RuntimeException);
}

TEST(SemanaTest, QueryTableScanSelectionMultipleExpressions) {
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo (a integer not null, b integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
            SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    std::istringstream in(R"SQLSCHEMA(
        select * from foo where a = 1234 and b = 2345;
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    std::cout << "Building statement..." << std::endl;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    std::cout << "Checking statement type..." << std::endl;
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
    std::cout << "Getting QueryStatement pointer..." << std::endl;
    auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
    std::cout << "Getting operator tree..." << std::endl;
    ASSERT_TRUE(stmtPtr->getTree());
    // the input operator should be a Print operator
    std::cout << "Getting operator tree..." << std::endl;
    auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
    ASSERT_TRUE(printPtr);
    std::cout << "Getting first Select operator..." << std::endl;
    auto selection1 = dynamic_cast<const imlab::algebra::Select*>(printPtr->getInput());
    ASSERT_TRUE(selection1);
    std::cout << "Getting second Select operator..." << std::endl;
    auto selection2 = dynamic_cast<const imlab::algebra::Select*>(selection1->getInput());
    ASSERT_TRUE(selection2);
    std::cout << "Getting TableScan operator..." << std::endl;
    auto tableScan = dynamic_cast<const imlab::algebra::TableScan*>(selection2->getInput());
    ASSERT_TRUE(tableScan);
}

TEST(SemanaTest, QueryJoin) {
    Database db;
    {
        // create table foo first, otherwise the semantic analysis does not find it
        std::istringstream in("create table foo(); create table bar();");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
            SemanticAnalysis s;
        for (auto& parseTree : parseTrees) {
            auto statement = s.buildStatement(*parseTree, db);
            ASSERT_TRUE(statement);
            statement->run(db);
        }
    }

    std::istringstream in(R"SQLQUERY(
        select * from foo, bar;
    )SQLQUERY");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    // run the semantic analysis on the parsed ast
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    // check the returned statement
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
    auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
    ASSERT_TRUE(stmtPtr->getTree());
    // the input operator should be a Print operator
    auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
    ASSERT_TRUE(printPtr);
    auto join = dynamic_cast<const imlab::algebra::InnerJoin*>(printPtr->getInput());
    ASSERT_TRUE(join);
    auto tableScan1 = dynamic_cast<const imlab::algebra::TableScan*>(join->getLeft());
    ASSERT_TRUE(tableScan1);
    auto tableScan2 = dynamic_cast<const imlab::algebra::TableScan*>(join->getRight());
    ASSERT_TRUE(tableScan2);
}

TEST(SemanaTest, SubqueryInFromClause) {
    Database db;
    {
        std::istringstream in("create table foo (a integer not null);");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
        SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);
    }

    {
        std::istringstream in("select * from (select * from foo) sub;");
        ParseContext spc;
        auto parseTrees = spc.Parse(in);
        ASSERT_EQ(parseTrees.size(), 1);
        SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::QueryStatementType);
        auto stmtPtr = static_cast<imlab::statement::QueryStatement*>(statement.get());
        ASSERT_TRUE(stmtPtr->getTree());
        auto printPtr = dynamic_cast<const imlab::algebra::Print*>(stmtPtr->getTree());
        ASSERT_TRUE(printPtr);
        auto subqueryScan = dynamic_cast<const imlab::algebra::SubqueryScan*>(printPtr->getInput());
        ASSERT_TRUE(subqueryScan);
        auto tableScan = dynamic_cast<const imlab::algebra::TableScan*>(subqueryScan->getSubquery());
        ASSERT_TRUE(tableScan);
        EXPECT_EQ(tableScan->getTableName(), "foo");
    }
}

using namespace imlab;

TEST(SemanaTest, SubqueryInFromClauseWithWhere) {
    Database db;
    {
        std::istringstream create_in("create table foo (a integer not null);");
        parser::ParseContext spc;
        auto parseTrees = spc.Parse(create_in);
        semana::SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        statement->run(db);

        db.insertRow("foo", {Value(Integer(1))});
        db.insertRow("foo", {Value(Integer(2))});
        db.insertRow("foo", {Value(Integer(3))});
        db.insertRow("foo", {Value(Integer(4))});
        db.insertRow("foo", {Value(Integer(5))});
    }

    {
        std::istringstream in(R"SQL(
            select *
            from (
                select * from foo where a = 1
            ) sub
            where sub.a = 1;
        )SQL");
        parser::ParseContext spc;
        auto parseTrees = spc.Parse(in);
        ASSERT_EQ(parseTrees.size(), 1);
        semana::SemanticAnalysis s;
        auto statement = s.buildStatement(*parseTrees[0], db);
        ASSERT_TRUE(statement);
        ASSERT_EQ(statement->getType(), statement::Statement::Type::QueryStatementType);
        auto stmtPtr = static_cast<statement::QueryStatement*>(statement.get());
        ASSERT_TRUE(stmtPtr->getTree());

        auto printPtr = dynamic_cast<const algebra::Print*>(stmtPtr->getTree());
        ASSERT_TRUE(printPtr);

        auto outerSelect = dynamic_cast<const algebra::Select*>(printPtr->getInput());
        ASSERT_TRUE(outerSelect) << "Outer where clause expected a Select operator.";

        auto subqueryScan = dynamic_cast<const algebra::SubqueryScan*>(outerSelect->getInput());
        ASSERT_TRUE(subqueryScan) << "Expected a SubqueryScan operator for the subquery.";

        auto innerSelect = dynamic_cast<const algebra::Select*>(subqueryScan->getSubquery());
        ASSERT_TRUE(innerSelect) << "Expected a Select operator inside the subquery for the inner where clause.";

        auto tableScan = dynamic_cast<const algebra::TableScan*>(innerSelect->getInput());
        ASSERT_TRUE(tableScan) << "Expected a TableScan operator under the inner Select.";
        EXPECT_EQ(tableScan->getTableName(), "foo");
    }
}



TEST(SemanaTest, CreateTableWithPrimaryKey) {
    std::istringstream in("create table bar (a integer not null, b integer not null, primary key (a, b));");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);

    Database db;
    SemanticAnalysis s;
    auto statement = s.buildStatement(*parseTrees[0], db);
    ASSERT_TRUE(statement);
    ASSERT_EQ(statement->getType(), imlab::statement::Statement::Type::CreateTableStatementType);
    auto stmtPtr = static_cast<imlab::statement::CreateTableStatement*>(statement.get());
    EXPECT_EQ(stmtPtr->getTable().id, "bar");

    stmtPtr->run(db);
    auto& table = stmtPtr->getTable();

    ASSERT_EQ(table.columns.size(), 2);
    ASSERT_EQ(table.primary_key.size(), 2);
    EXPECT_EQ(table.primary_key[0].id, "a");
    EXPECT_EQ(table.primary_key[1].id, "b");
}



} // namespace test