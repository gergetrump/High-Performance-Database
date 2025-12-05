// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/Database.h"
#include "gtest/gtest.h"
#include "imlab/algebra/IU.h"
#include "imlab/semana/SemanticAnalysis.h"
#include "imlab/statement/CopyTableStatement.h"
#include "imlab/statement/CreateTableStatement.h"
#include "imlab/statement/Statement.h"
#include "test/tpcc/Data.h"
#include "tools/statictpcc/TPCC.h"
#include <array>
#include <fstream>
#include <sstream>
//---------------------------------------------------------------------------
using namespace imlab;
//---------------------------------------------------------------------------
namespace test {
//---------------------------------------------------------------------------
#define MakeTest(T)                                 \
    TEST(TPCCTest, Load##T) {                       \
        statictpcc::TPCC db;                        \
        std::stringstream in(imlab_test::kTest##T); \
        db.Load##T(in);                             \
        ASSERT_EQ(db.Size<statictpcc::k##T>(), 5);  \
    }

//---------------------------------------------------------------------------
// NOLINT BEGIN
// clang-format off
MakeTest(Warehouse)
MakeTest(District)
MakeTest(History)
MakeTest(Item)
MakeTest(NewOrder)
MakeTest(Order)
MakeTest(OrderLine)
MakeTest(Stock)
MakeTest(Customer)

    // clang-format on
    // NOLINT END
    // ---------------------------------------------------------------------------
    TEST(TPCCTestLocal, NewOrder) {
    statictpcc::TPCC data;
    data.LoadTPCC();

    ASSERT_EQ(data.Size<statictpcc::kWarehouse>(), 5);
    ASSERT_EQ(data.Size<statictpcc::kDistrict>(), 50);
    ASSERT_EQ(data.Size<statictpcc::kHistory>(), 150000);
    ASSERT_EQ(data.Size<statictpcc::kItem>(), 100000);
    ASSERT_EQ(data.Size<statictpcc::kNewOrder>(), 45000);
    ASSERT_EQ(data.Size<statictpcc::kOrder>(), 150000);
    ASSERT_EQ(data.Size<statictpcc::kOrderLine>(), 1425564);
    ASSERT_EQ(data.Size<statictpcc::kStock>(), 500000);
    ASSERT_EQ(data.Size<statictpcc::kCustomer>(), 150000);

    std::array<Integer, 15> supware;
    supware[0] = Integer(2);
    supware[1] = Integer(3);
    supware[2] = Integer(4);
    supware[3] = Integer(1);
    std::array<Integer, 15> itemid;
    itemid[0] = Integer(2000);
    itemid[1] = Integer(4000);
    itemid[2] = Integer(6000);
    itemid[3] = Integer(8000);
    std::array<Integer, 15> qty;
    qty[0] = Integer(2);
    qty[1] = Integer(4);
    qty[2] = Integer(6);
    qty[3] = Integer(8);

    statictpcc::NewOrder(data, Integer(4), Integer(1), Integer(1), Integer(4), supware, itemid, qty, Timestamp(0));

    ASSERT_EQ(data.Size<statictpcc::kWarehouse>(), 5);
    ASSERT_EQ(data.Size<statictpcc::kDistrict>(), 50);
    ASSERT_EQ(data.Size<statictpcc::kHistory>(), 150000);
    ASSERT_EQ(data.Size<statictpcc::kItem>(), 100000);

    ASSERT_EQ(data.Size<statictpcc::kNewOrder>(), 45000 + 1);
    // neworder: inserted tuple [no_o_id = 3001, no_d_id = 1, no_w_id = 4]
    //---
    // (student) add index lookup for no_o_id = 3001, no_d_id = 1, no_w_id = 4 to check correctness of inserted tuple (check the tuple can be looked up)
    auto neworder_iter = data.neworder_ids.find(std::make_tuple(Integer(4), Integer(1), Integer(3001)));
    ASSERT_NE(neworder_iter, data.neworder_ids.end());

    const auto& neworder_row = data.neworder_rows[static_cast<size_t>(neworder_iter->second.value)];
    ASSERT_EQ(neworder_row.no_w_id.value, 4);
    ASSERT_EQ(neworder_row.no_d_id.value, 1);
    ASSERT_EQ(neworder_row.no_o_id.value, 3001);
    //---

    ASSERT_EQ(data.Size<statictpcc::kOrder>(), 150000 + 1);
    // order: inserted tuple [o_o_id = 3001, o_d_id = 1, o_w_id = 4, o_c_id = 1, o_entry_d = 0, o_carrier_id = 0, o_ol_cnt = 4, o_all_local = 0]
    //---
    // (student) add index lookup for o_o_id = 3001, o_d_id = 1, o_w_id = 4 to check correctness of inserted tuple (check value of o_ol_cnt)
    auto order_iter = data.order_ids.find(std::make_tuple(Integer(4), Integer(1), Integer(3001)));
    ASSERT_NE(order_iter, data.order_ids.end());

    const auto& order_row = data.order_rows[static_cast<size_t>(order_iter->second.value)];
    ASSERT_EQ(order_row.o_w_id.value, 4);
    ASSERT_EQ(order_row.o_d_id.value, 1);
    ASSERT_EQ(order_row.o_id.value, 3001);
    ASSERT_EQ(order_row.o_c_id.value, 1);
    ASSERT_EQ(order_row.o_entry_d.value, 0);
    ASSERT_EQ(order_row.o_carrier_id.value, 0);
    ASSERT_EQ(order_row.o_ol_cnt.value, 4);
    ASSERT_EQ(order_row.o_all_local.value, 0);
    //---


    ASSERT_EQ(data.Size<statictpcc::kOrderLine>(), 1425564 + 4);
    // orderline: inserted tuples (ol_amount might differ to calculations by hand or postgres by 1-2 cents)
    // [ol_o_id = 3001, ol_d_id = 1, ol_w_id = 4, ol_number = 1, ol_i_id = 2000, ol_supply_w_id = 2, ol_delivery_d = 0, ol_quantity = 2, ol_amount = 103.88, ol_dist_info = BNtvegAMKuMUjTFs4DsVSpcN]
    // [ol_o_id = 3001, ol_d_id = 1, ol_w_id = 4, ol_number = 2, ol_i_id = 4000, ol_supply_w_id = 3, ol_delivery_d = 0, ol_quantity = 4, ol_amount = 47.73, ol_dist_info = qPGVnI55KQLBPVsupnpx2DVv]
    // [ol_o_id = 3001, ol_d_id = 1, ol_w_id = 4, ol_number = 3, ol_i_id = 6000, ol_supply_w_id = 4, ol_delivery_d = 0, ol_quantity = 6, ol_amount = 287.71, ol_dist_info = KMBpiXpYSwaMYEYlN4FGtwsa]
    // [ol_o_id = 3001, ol_d_id = 1, ol_w_id = 4, ol_number = 4, ol_i_id = 8000, ol_supply_w_id = 1, ol_delivery_d = 0, ol_quantity = 8, ol_amount = 247.27, ol_dist_info = OGRIrXrkKCQkL2Y4aZILws20]

    //---
    // (student) add check for the correct insertion of these tuples (check the value of ol_quantity)
    for (int i = 0; i < 4; i++) {
        auto orderline_iter = data.orderline_ids.find(std::make_tuple(Integer(4), Integer(1), Integer(3001), Integer(i+1)));
        ASSERT_NE(orderline_iter, data.orderline_ids.end());

        const auto& orderline_row = data.orderline_rows[static_cast<size_t>(orderline_iter->second.value)];
        ASSERT_EQ(orderline_row.ol_w_id.value, 4);
        ASSERT_EQ(orderline_row.ol_d_id.value, 1);
        ASSERT_EQ(orderline_row.ol_o_id.value, 3001);
        ASSERT_EQ(orderline_row.ol_number.value, i+1);
        ASSERT_EQ(orderline_row.ol_i_id.value, itemid[i].value);
        ASSERT_EQ(orderline_row.ol_supply_w_id.value, supware[i].value);
        ASSERT_EQ(orderline_row.ol_delivery_d.value, 0);
        ASSERT_EQ(orderline_row.ol_quantity.value, qty[i].value);
    }

    //---

    ASSERT_EQ(data.Size<statictpcc::kStock>(), 500000);
    ASSERT_EQ(data.Size<statictpcc::kCustomer>(), 150000);
}
//---------------------------------------------------------------------------
class CaptureCout {
    private:
    std::streambuf* sbuf;

    public:
    std::stringstream stream;

    CaptureCout() : sbuf(std::cout.rdbuf()) {
        std::cout.rdbuf(stream.rdbuf());
    }

    ~CaptureCout() {
        std::cout.rdbuf(sbuf);
    }
};
//---------------------------------------------------------------------------
class SchemaMapping {
    std::unordered_map<std::string, parser::Table> table;

    public:
    SchemaMapping(const std::vector<parser::Table>& data) {
        for (auto& entry : data)
            table.insert({entry.id, entry});
    }

    bool contains(auto d) const { return table.contains(d); }
    auto find(auto d) const { return table.find(d); }
    auto begin() const { return table.begin(); }
    auto end() const { return table.end(); }
    auto size() const { return table.size(); }
};
using DataMapping = std::unordered_map<std::string, const char*>;
//---------------------------------------------------------------------------
struct Mapping {
    SchemaMapping schema;
    DataMapping data;
};
//---------------------------------------------------------------------------
// simplified tpcc schema (without primary keys)
static const std::vector<parser::Table> tpccSchemaVec = {
    {"neworder", {{"no_o_id", imlab::Type::Integer()}, {"no_d_id", imlab::Type::Integer()}, {"no_w_id", imlab::Type::Integer()}}, {{"no_w_id", imlab::Type::Integer()}, {"no_d_id", imlab::Type::Integer()}, {"no_o_id", imlab::Type::Integer()}}},
    {"stock", {{"s_i_id", imlab::Type::Integer()}, {"s_w_id", imlab::Type::Integer()}, {"s_quantity", imlab::Type::Numeric(4, 0)}, {"s_dist_01", imlab::Type::Char(24)}, {"s_dist_02", imlab::Type::Char(24)}, {"s_dist_03", imlab::Type::Char(24)}, {"s_dist_04", imlab::Type::Char(24)}, {"s_dist_05", imlab::Type::Char(24)}, {"s_dist_06", imlab::Type::Char(24)}, {"s_dist_07", imlab::Type::Char(24)}, {"s_dist_08", imlab::Type::Char(24)}, {"s_dist_09", imlab::Type::Char(24)}, {"s_dist_10", imlab::Type::Char(24)}, {"s_ytd", imlab::Type::Numeric(8, 0)}, {"s_order_cnt", imlab::Type::Numeric(4, 0)}, {"s_remote_cnt", imlab::Type::Numeric(4, 0)}, {"s_data", imlab::Type::Varchar(50)}}, {{"s_w_id", imlab::Type::Integer()}, {"s_i_id", imlab::Type::Integer()}}},
    {"orderline", {{"ol_o_id", imlab::Type::Integer()}, {"ol_d_id", imlab::Type::Integer()}, {"ol_w_id", imlab::Type::Integer()}, {"ol_number", imlab::Type::Integer()}, {"ol_i_id", imlab::Type::Integer()}, {"ol_supply_w_id", imlab::Type::Integer()}, {"ol_delivery_d", imlab::Type::Timestamp()}, {"ol_quantity", imlab::Type::Numeric(2, 0)}, {"ol_amount", imlab::Type::Numeric(6, 2)}, {"ol_dist_info", imlab::Type::Char(24)}}, {{"ol_w_id", imlab::Type::Integer()}, {"ol_d_id", imlab::Type::Integer()}, {"ol_o_id", imlab::Type::Integer()}, {"ol_number", imlab::Type::Integer()}}},
    {"customer", {{"c_id", imlab::Type::Integer()}, {"c_d_id", imlab::Type::Integer()}, {"c_w_id", imlab::Type::Integer()}, {"c_first", imlab::Type::Varchar(16)}, {"c_middle", imlab::Type::Char(2)}, {"c_last", imlab::Type::Varchar(16)}, {"c_street_1", imlab::Type::Varchar(20)}, {"c_street_2", imlab::Type::Varchar(20)}, {"c_city", imlab::Type::Varchar(20)}, {"c_state", imlab::Type::Char(2)}, {"c_zip", imlab::Type::Char(9)}, {"c_phone", imlab::Type::Char(16)}, {"c_since", imlab::Type::Timestamp()}, {"c_credit", imlab::Type::Char(2)}, {"c_credit_lim", imlab::Type::Numeric(12, 2)}, {"c_discount", imlab::Type::Numeric(4, 4)}, {"c_balance", imlab::Type::Numeric(12, 2)}, {"c_ytd_paymenr", imlab::Type::Numeric(12, 2)}, {"c_payment_cnt", imlab::Type::Numeric(4, 0)}, {"c_delivery_cnt", imlab::Type::Numeric(4, 0)}, {"c_data", imlab::Type::Varchar(500)}}, {{"c_w_id", imlab::Type::Integer()}, {"c_d_id", imlab::Type::Integer()}, {"c_id", imlab::Type::Integer()}}},
    {"order", {{"o_id", imlab::Type::Integer()}, {"o_d_id", imlab::Type::Integer()}, {"o_w_id", imlab::Type::Integer()}, {"o_c_id", imlab::Type::Integer()}, {"o_entry_d", imlab::Type::Timestamp()}, {"o_carrier_id", imlab::Type::Integer()}, {"o_ol_cnt", imlab::Type::Numeric(2, 0)}, {"o_all_local", imlab::Type::Numeric(1, 0)}}, {{"o_w_id", imlab::Type::Integer()}, {"o_d_id", imlab::Type::Integer()}, {"o_id", imlab::Type::Integer()}}},
    {"history", {{"h_c_id", imlab::Type::Integer()}, {"h_c_d_id", imlab::Type::Integer()}, {"h_c_w_id", imlab::Type::Integer()}, {"h_d_id", imlab::Type::Integer()}, {"h_w_id", imlab::Type::Integer()}, {"h_date", imlab::Type::Timestamp()}, {"h_amount", imlab::Type::Numeric(6, 2)}, {"h_data", imlab::Type::Varchar(24)}}, {}},
    {"district", {{"d_id", imlab::Type::Integer()}, {"d_w_id", imlab::Type::Integer()}, {"d_name", imlab::Type::Varchar(10)}, {"d_street_1", imlab::Type::Varchar(20)}, {"d_street_2", imlab::Type::Varchar(20)}, {"d_city", imlab::Type::Varchar(20)}, {"d_state", imlab::Type::Char(2)}, {"d_zip", imlab::Type::Char(9)}, {"d_tax", imlab::Type::Numeric(4, 4)}, {"d_ytd", imlab::Type::Numeric(12, 2)}, {"d_next_o_id", imlab::Type::Integer()}}, {{"d_w_id", imlab::Type::Integer()}, {"d_id", imlab::Type::Integer()}}},
    {"item", {{"i_id", imlab::Type::Integer()}, {"i_im_id", imlab::Type::Integer()}, {"i_name", imlab::Type::Varchar(24)}, {"i_price", imlab::Type::Numeric(5, 2)}, {"i_data", imlab::Type::Varchar(50)}}, {{"i_id", imlab::Type::Integer()}}},
    {"warehouse", {{"w_id", imlab::Type::Integer()}, {"w_name", imlab::Type::Varchar(10)}, {"w_street_1", imlab::Type::Varchar(20)}, {"w_street_2", imlab::Type::Varchar(20)}, {"w_city", imlab::Type::Varchar(20)}, {"w_state", imlab::Type::Char(2)}, {"w_zip", imlab::Type::Char(9)}, {"w_tax", imlab::Type::Numeric(4, 4)}, {"w_ytd", imlab::Type::Numeric(12, 2)}}, {{"w_id", imlab::Type::Integer()}}}};
static const SchemaMapping tpccSchema(tpccSchemaVec);
//---------------------------------------------------------------------------
// mapping of the tpcc table to the test data
static const DataMapping tpccTestData = {
    {"neworder", imlab_test::kTestNewOrder},
    {"stock", imlab_test::kTestStock},
    {"orderline", imlab_test::kTestOrderLine},
    {"customer", imlab_test::kTestCustomer},
    {"order", imlab_test::kTestOrder},
    {"history", imlab_test::kTestHistory},
    {"district", imlab_test::kTestDistrict},
    {"item", imlab_test::kTestItem},
    {"warehouse", imlab_test::kTestWarehouse},
};
//---------------------------------------------------------------------------
static const std::unordered_map<std::string, const Mapping> info = {
    {"tpcc", {tpccSchema, tpccTestData}}};
//---------------------------------------------------------------------------
void createAndLoadTable(imlab::Database& db, const std::string& schema, const std::string& tableName)
// this function creates the table, loads data to it, and check its size
{
    assert(info.contains(schema));
    auto& entry = info.find(schema)->second;
    auto& schemaInfo = entry.schema;
    auto& data = entry.data;
    assert(schemaInfo.contains(tableName));
    {
        // register the table in the database
        auto localTable = schemaInfo.find(tableName)->second;
        statement::CreateTableStatement c(localTable);
        c.run(db);
    }

    // flush the test data to a temporary file and run the copy command
    std::string file = "";
    // read the test data
    assert(data.contains(tableName));
    std::string name = "imlabtemp";
    {
        std::ofstream f(name);
        f << data.find(tableName)->second;
        file = name;
    }

    {
        // Load the data to the table
        assert(file != "");
        statement::CopyTableStatement c(tableName, file, "|");
        if (tableName == "district") {
            int test = 0;
        }
        c.run(db);
    }
}
//---------------------------------------------------------------------------
void loadTPCC(imlab::Database& db) {
    std::for_each(tpccSchemaVec.begin(), tpccSchemaVec.end(), [&db](auto& iter) {
        createAndLoadTable(db, "tpcc", iter.id);
    });
}
//---------------------------------------------------------------------------
std::string runQuery(imlab::Database& db, std::string query) {
    std::cout << "running: " << query << std::endl;
    CaptureCout cout;
    std::stringstream s;
    s << query << ";";
    parser::ParseContext pc;
    auto r = pc.Parse(s);
    std::vector<std::unique_ptr<statement::Statement>> statements;
    statements.reserve(r.size());
    semana::SemanticAnalysis semana;
    for (auto& ast : r) {
        try {
            auto statement = semana.buildStatement(*ast, db);
            statement->run(db);
        } catch (imlab::RuntimeException& e) {
            std::cerr << e.getMessage();
            assert(false);
        } catch (std::exception& e) {
            throw e;
        }
    }
    return cout.stream.str();
}
//---------------------------------------------------------------------------
TEST(DatabaseTest, Query) {
    imlab::Database db;
    loadTPCC(db);

    {
        std::string expected = "4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n"
                               "1, 2Xtbfe, XlZDbdUdAn6B9wq5qmzV, 8SGj3S1diu, qFlC5kZz3Rk, Nb, 657611111, 0.1923, 3000000.00\n"
                               "5, f616uFN, sEZcx4foeppu, NZgdLndt5qAE4btW9H, 0hoEsJKQTC7XoM, oG, 334311111, 0.1938, 3000000.00\n"
                               "2, hbgihp9pfk, 8Uheuf5hvj3YjQHXNVLr, xPUx0Y5ICDEr, dClA1jnPNt, vL, 470911111, 0.1040, 3000000.00\n"
                               "3, z0Y98H, SLtA4eJWLXusdTB, 4kmgyhsiT6xKR505F, YFmTwj83737pFjSX, jy, 299411111, 0.1157, 3000000.00\n";
        std::string result = runQuery(db, "select * from warehouse");
        // delete the column names
        result.erase(0, result.find('\n') + 1);
        ASSERT_EQ(result, expected);
    }
    {
        std::string expected = "5, f616uFN, sEZcx4foeppu, NZgdLndt5qAE4btW9H, 0hoEsJKQTC7XoM, oG, 334311111, 0.1938, 3000000.00\n";
        std::string result = runQuery(db, "select * from warehouse where w_id = 5");
        // delete the column names
        result.erase(0, result.find('\n') + 1);
        ASSERT_EQ(result, expected);
    }
    {
        std::string expected = "2101, 1, 4, 4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n"
                               "2102, 1, 4, 4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n"
                               "2103, 1, 4, 4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n"
                               "2104, 1, 4, 4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n"
                               "2105, 1, 4, 4, LPESIfo, aiAcXSImDqLf, wVqSpx1e3DNVdD3mo, 2vIomPtgo1U3PJUgHYr, bw, 584811111, 0.1973, 3000000.00\n";
        std::string result = runQuery(db, "select * from neworder, warehouse where no_w_id = w_id");
        // delete the column names
        result.erase(0, result.find('\n') + 1);
        ASSERT_EQ(result, expected);
    }
}
//---------------------------------------------------------------------------
} // namespace test
//---------------------------------------------------------------------------