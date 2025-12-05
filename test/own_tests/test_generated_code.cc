#include "../../imlab/Database.h"
#include "../../imlab/statement/QueryStatement.h"
#include "../../imlab/algebra/Operator.h"
#include "../../imlab/algebra/TableScan.h"
#include "../../imlab/algebra/Select.h"
#include "../../imlab/algebra/Print.h"
#include "../../imlab/algebra/Expression.h"
#include "../../imlab/algebra/IURef.h"
#include "../../imlab/algebra/Const.h"
#include "../../imlab/algebra/BinaryExpression.h"
#include "../../imlab/algebra/IU.h"
#include "../../imlab/infra/Types.h"
#include "../../imlab/types/Types.h"
#include "imlab/infra/HashTable.h"

#include <fstream>
#include <memory>
#include <vector>
#include <iostream>
#include <imlab/algebra/InnerJoin.h>
#include <imlab/parser/gen/parser.h>
#include <imlab/statement/CopyTableStatement.h>
#include <imlab/statement/CreateTableStatement.h>
#include <tbb/parallel_for.h>


int testSimpleQuery() {
    imlab::Database db;

    imlab::parser::Table warehouseSchema;
    warehouseSchema.id = "Warehouse";
    warehouseSchema.columns.push_back({"w_id", imlab::Type::Integer()});
    warehouseSchema.columns.push_back({"w_name", imlab::Type::Varchar(10)});

    db.createTable(warehouseSchema);

    db.insertRow("Warehouse", {imlab::Value(imlab::Integer(1)), imlab::Value("Warehouse One", imlab::Type::Varchar(10))});

    db.insertRow("Warehouse", {imlab::Value(imlab::Integer(2)), imlab::Value("Warehouse Two", imlab::Type::Varchar(10))});

    const imlab::parser::Table& tableSchema = db.getTable("Warehouse").getSchema();

    const auto &columnIUs = db.getTable("Warehouse").getColumnsIUs();
    std::vector<const imlab::algebra::IU*> iuPtrs;
    iuPtrs.reserve(columnIUs.size());
    for (const auto &iuPtr : columnIUs) {
        iuPtrs.push_back(iuPtr.get());
    }
    auto warehouseScan = std::make_unique<imlab::algebra::TableScan>("Warehouse", 0, tableSchema, iuPtrs);

    const auto& tableScanIUs = warehouseScan->getRegisteredIUs();
    const imlab::algebra::IU* w_id_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_id");
    const imlab::algebra::IU* w_name_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_name");

    auto w_id_ref = std::make_unique<imlab::algebra::IURef>(w_id_iu);
    auto const_one = std::make_unique<imlab::algebra::Const>(imlab::Value(imlab::Integer(1)), w_id_iu->type_);
    auto condition = std::make_unique<imlab::algebra::BinaryExpression>(
        imlab::algebra::BinaryExpression::Op::Equal,
        std::move(w_id_ref),
        std::move(const_one)
    );

    std::unique_ptr<imlab::algebra::Select> selectOp = std::make_unique<imlab::algebra::Select>(
        std::move(warehouseScan),
        std::move(condition)
    );

    std::vector<imlab::algebra::Print::Output> outputs;
    outputs.emplace_back("w_name", std::make_unique<imlab::algebra::IURef>(w_name_iu));


    auto printOp = std::make_unique<imlab::algebra::Print>(
        std::move(selectOp),
        std::move(outputs)
    );

    imlab::statement::QueryStatement queryStmt(std::move(printOp));

    try {
        queryStmt.run(db);
    } catch (const std::exception& ex) {
        std::cerr << "Error executing query: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "testSimpleQuery passed." << std::endl;
    return 0;
}

int testComplexQuery() {
    imlab::Database db;

    imlab::parser::Table warehouseSchema;
    warehouseSchema.id = "Warehouse";
    warehouseSchema.columns.push_back({"w_id", imlab::Type::Integer()});
    warehouseSchema.columns.push_back({"w_name", imlab::Type::Varchar(10)});

    imlab::parser::Table districtSchema;
    districtSchema.id = "District";
    districtSchema.columns.push_back({"d_id", imlab::Type::Integer()});
    districtSchema.columns.push_back({"w_id", imlab::Type::Integer()});
    districtSchema.columns.push_back({"d_name", imlab::Type::Varchar(10)});

    db.createTable(warehouseSchema);
    db.createTable(districtSchema);

    db.insertRow("Warehouse", {imlab::Value(imlab::Integer(1)), imlab::Value("Warehouse One", imlab::Type::Varchar(10))});
    db.insertRow("Warehouse", {imlab::Value(imlab::Integer(2)), imlab::Value("Warehouse Two", imlab::Type::Varchar(10))});

    db.insertRow("District", {imlab::Value(imlab::Integer(10)), imlab::Value(imlab::Integer(1)), imlab::Value("District A", imlab::Type::Varchar(10))});
    db.insertRow("District", {imlab::Value(imlab::Integer(20)), imlab::Value(imlab::Integer(2)), imlab::Value("District B", imlab::Type::Varchar(10))});

    const auto& warehouseSchemaRef = db.getTable("Warehouse").getSchema();
    const auto &warehouseColumnIUs = db.getTable("Warehouse").getColumnsIUs();
    std::vector<const imlab::algebra::IU*> warehouseIUPtrs;
    warehouseIUPtrs.reserve(warehouseColumnIUs.size());
    for (const auto &iuPtr : warehouseColumnIUs) {
        warehouseIUPtrs.push_back(iuPtr.get());
    }

    auto warehouseScan = std::make_unique<imlab::algebra::TableScan>("Warehouse", 0, warehouseSchemaRef, warehouseIUPtrs);
    const auto& warehouseIUs = warehouseScan->getRegisteredIUs();
    const imlab::algebra::IU* w_id_iu = imlab::algebra::TableScan::getIUByName(warehouseIUs, "w_id");
    const imlab::algebra::IU* w_name_iu = imlab::algebra::TableScan::getIUByName(warehouseIUs, "w_name");

    const auto& districtSchemaRef = db.getTable("District").getSchema();
    const auto &districtColumnIUs = db.getTable("District").getColumnsIUs();
    std::vector<const imlab::algebra::IU*> districtIUPtrs;
    districtIUPtrs.reserve(districtColumnIUs.size());
    for (const auto &iuPtr : districtColumnIUs) {
        districtIUPtrs.push_back(iuPtr.get());
    }

    auto districtScan = std::make_unique<imlab::algebra::TableScan>("District", 1, districtSchemaRef, districtIUPtrs);
    const auto& districtIUs = districtScan->getRegisteredIUs();
    const imlab::algebra::IU* d_id_iu = imlab::algebra::TableScan::getIUByName(districtIUs, "d_id");
    const imlab::algebra::IU* d_w_id_iu = imlab::algebra::TableScan::getIUByName(districtIUs, "w_id");
    const imlab::algebra::IU* d_name_iu = imlab::algebra::TableScan::getIUByName(districtIUs, "d_name");

    auto w_id_ref = std::make_unique<imlab::algebra::IURef>(w_id_iu);
    auto d_w_id_ref = std::make_unique<imlab::algebra::IURef>(d_w_id_iu);
    auto joinCondition = std::make_unique<imlab::algebra::BinaryExpression>(
        imlab::algebra::BinaryExpression::Op::Equal,
        std::move(w_id_ref),
        std::move(d_w_id_ref)
    );

    auto joinOp = std::make_unique<imlab::algebra::InnerJoin>(
        std::move(warehouseScan),
        std::move(districtScan),
        std::move(joinCondition)
    );

    auto d_id_ref = std::make_unique<imlab::algebra::IURef>(d_id_iu);
    auto const_ten = std::make_unique<imlab::algebra::Const>(imlab::Value(imlab::Integer(10)), d_id_iu->type_);
    auto selectionCondition = std::make_unique<imlab::algebra::BinaryExpression>(
        imlab::algebra::BinaryExpression::Op::Equal,
        std::move(d_id_ref),
        std::move(const_ten)
    );

    auto selectOp = std::make_unique<imlab::algebra::Select>(
        std::move(joinOp),
        std::move(selectionCondition)
    );

    std::vector<imlab::algebra::Print::Output> outputs;
    outputs.emplace_back("w_name", std::make_unique<imlab::algebra::IURef>(w_name_iu));
    outputs.emplace_back("d_name", std::make_unique<imlab::algebra::IURef>(d_name_iu));

    auto printOp = std::make_unique<imlab::algebra::Print>(
        std::move(selectOp),
        std::move(outputs)
    );

    imlab::statement::QueryStatement queryStmt(std::move(printOp));

    try {
        queryStmt.run(db);
    } catch (const std::exception& ex) {
        std::cerr << "Error executing query: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "testComplexQuery passed." << std::endl;
    return 0;
}


void testLazyMultiMap() {
    LazyMultiMap<Key<int>, std::string> map;

    map.insert({Key(1), "A"});
    map.insert({Key(2), "B"});
    map.insert({Key(2), "C"});
    map.insert({Key(3), "D"});
    map.insert({Key(2), "E"});

    map.finalize();

    auto range = map.equal_range(Key(2));
    std::vector<std::string> values;
    for (auto it = range.first; it != range.second; ++it) {
        values.push_back(it->value_);
    }

    assert(values.size() == 3);
    bool hasB = false, hasC = false, hasE = false;
    for (auto &val : values) {
        if (val == "B") hasB = true;
        if (val == "C") hasC = true;
        if (val == "E") hasE = true;
    }
    assert(hasB && hasC && hasE);

    range = map.equal_range(Key(1));
    assert(range.first != range.second);
    assert(range.first->value_ == "A");

    range = map.equal_range(Key(99));
    assert(range.first == range.second);

    std::cout << "testLazyMultiMap passed." << std::endl;
}

int testCopyTable() {
    imlab::Database db;

    imlab::parser::Table warehouseSchema;
    warehouseSchema.id = "Warehouse";
    warehouseSchema.columns.push_back({"w_id", imlab::Type::Integer()});
    warehouseSchema.columns.push_back({"w_name", imlab::Type::Varchar(30)});

    db.createTable(warehouseSchema);

    {
        std::ofstream file("test_warehouse_data");
        file << "1|Warehouse_One\n";
        file << "2|Warehouse_Two\n";
    }

    {
        imlab::statement::CopyTableStatement copyStmt("Warehouse", "test_warehouse_data", "|");
        copyStmt.run(db);
    }

    const auto &tableSchema = db.getTable("Warehouse").getSchema();
    const auto &columnIUs = db.getTable("Warehouse").getColumnsIUs();
    std::vector<const imlab::algebra::IU*> iuPtrs;
    iuPtrs.reserve(columnIUs.size());
    for (const auto &iuPtr : columnIUs) {
        iuPtrs.push_back(iuPtr.get());
    }
    auto warehouseScan = std::make_unique<imlab::algebra::TableScan>("Warehouse", 0, tableSchema, iuPtrs);
    const auto &tableScanIUs = warehouseScan->getRegisteredIUs();
    const imlab::algebra::IU* w_id_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_id");
    const imlab::algebra::IU* w_name_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_name");

    std::vector<imlab::algebra::Print::Output> outputs;
    outputs.emplace_back("w_id", std::make_unique<imlab::algebra::IURef>(w_id_iu));
    outputs.emplace_back("w_name", std::make_unique<imlab::algebra::IURef>(w_name_iu));

    auto printOp = std::make_unique<imlab::algebra::Print>(std::move(warehouseScan), std::move(outputs));

    imlab::statement::QueryStatement queryStmt(std::move(printOp));

    queryStmt.run(db);

    std::cout << "testCopyTable passed." << std::endl;
    return 0;
}

int testCreateTable() {
    imlab::Database db;

    imlab::parser::Table warehouseSchema;
    warehouseSchema.id = "Warehouse";
    warehouseSchema.columns.push_back({"w_id", imlab::Type::Integer()});
    warehouseSchema.columns.push_back({"w_name", imlab::Type::Varchar(30)});

    {
        imlab::statement::CreateTableStatement createStmt(warehouseSchema);
        createStmt.run(db);
    }

    db.insertRow("Warehouse", {imlab::Value(imlab::Integer(1)), imlab::Value("Warehouse_One", imlab::Type::Varchar(30))});

    const auto &tableSchema = db.getTable("Warehouse").getSchema();

    const auto &columnIUs = db.getTable("Warehouse").getColumnsIUs();
    std::vector<const imlab::algebra::IU*> iuPtrs;
    iuPtrs.reserve(columnIUs.size());
    for (const auto &iuPtr : columnIUs) {
        iuPtrs.push_back(iuPtr.get());
    }

    auto warehouseScan = std::make_unique<imlab::algebra::TableScan>("Warehouse", 0, tableSchema, iuPtrs);
    const auto &tableScanIUs = warehouseScan->getRegisteredIUs();

    const imlab::algebra::IU* w_id_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_id");
    const imlab::algebra::IU* w_name_iu = imlab::algebra::TableScan::getIUByName(tableScanIUs, "w_name");

    std::vector<imlab::algebra::Print::Output> outputs;
    outputs.emplace_back("w_id", std::make_unique<imlab::algebra::IURef>(w_id_iu));
    outputs.emplace_back("w_name", std::make_unique<imlab::algebra::IURef>(w_name_iu));

    auto printOp = std::make_unique<imlab::algebra::Print>(std::move(warehouseScan), std::move(outputs));
    imlab::statement::QueryStatement queryStmt(std::move(printOp));

    try {
        queryStmt.run(db);
    } catch (const std::exception& ex) {
        std::cerr << "Error executing query: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "testCreateTable passed." << std::endl;
    return 0;
}


void testHashJoinWithLazyMultiMap() {
    // testing and understanding whether the logical implementation of hash join works.

    // Prepare data

    imlab::parser::Table leftSchema;
    leftSchema.id = "leftTable";
    leftSchema.columns.push_back({"l_id", imlab::Type::Integer()});
    leftSchema.columns.push_back({"l_name", imlab::Type::Varchar(30)});
    imlab::Table leftTable(leftSchema);

    imlab::parser::Table rightSchema;
    rightSchema.id = "rightTable";
    rightSchema.columns.push_back({"r_id", imlab::Type::Integer()});
    rightSchema.columns.push_back({"r_name", imlab::Type::Varchar(30)});
    imlab::Table rightTable(rightSchema);

    leftTable.insertRow({imlab::Value(imlab::Integer(1)), imlab::Value("Alice", imlab::Type::Varchar(30))});
    leftTable.insertRow({imlab::Value(imlab::Integer(2)), imlab::Value("Bob", imlab::Type::Varchar(30))});
    leftTable.insertRow({imlab::Value(imlab::Integer(2)), imlab::Value("Carl", imlab::Type::Varchar(30))});
    leftTable.insertRow({imlab::Value(imlab::Integer(3)), imlab::Value("David", imlab::Type::Varchar(30))});

    rightTable.insertRow({imlab::Value(imlab::Integer(2)), imlab::Value("Alex", imlab::Type::Varchar(30))});
    rightTable.insertRow({imlab::Value(imlab::Integer(4)), imlab::Value("Ben", imlab::Type::Varchar(30))});
    rightTable.insertRow({imlab::Value(imlab::Integer(2)), imlab::Value("Chris", imlab::Type::Varchar(30))});
    rightTable.insertRow({imlab::Value(imlab::Integer(1)), imlab::Value("Diana", imlab::Type::Varchar(30))});

    // Build phase

    LazyMultiMap<Key<imlab::Value>, std::vector<imlab::Value>> hashTable;

    size_t leftKeyIndex = leftTable.getColumnIndices().at("l_id");
    for (const auto& row : leftTable.getRows()) {
        imlab::Value leftKey = row[leftKeyIndex];
        hashTable.insert({Key(leftKey), row});
    }
    hashTable.finalize();

    // Probe phase

    std::vector<std::vector<imlab::Value>> joinedTable;
    size_t rightKeyIndex = rightTable.getColumnIndices().at("r_id");
    for (const auto& row : rightTable.getRows()) {
        imlab::Value rightKey = row[rightKeyIndex];
        auto [start, end] = hashTable.equal_range(Key(rightKey));

        for (auto iter = start; iter != end; ++iter) {
            const auto& leftRow = iter->value_; // get the matching left row
            std::vector<imlab::Value> joinedRow = leftRow; // start with left row
            joinedRow.insert(joinedRow.end(), row.begin(), row.end()); // append the right row
            joinedTable.push_back(joinedRow); // save the joined row
        }
    }

    // Verify results

    assert(joinedTable.size() == 5);

    std::cout << "Joined Results:\n";
    for (const auto& row : joinedTable) {
        for (const auto& val : row) {
            if (val.getType().getClass() == imlab::Type::kInteger) {
                std::cout << val.getInteger().value << " ";
            } else if (val.getType().getClass() == imlab::Type::kVarchar) {
                std::cout << val.getStringValue() << " ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "testHashJoinWithLazyMultiMap passed." << std::endl;

}


int main() {
    testSimpleQuery();
    testComplexQuery();
    testLazyMultiMap();
    testCopyTable();
    testCreateTable();
    testHashJoinWithLazyMultiMap();
}

