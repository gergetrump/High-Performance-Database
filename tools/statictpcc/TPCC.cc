//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#include "TPCC.h"
#include "imlab/types/Types.h"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

//---------------------------------------------------------------------------
using namespace imlab;
//---------------------------------------------------------------------------
namespace statictpcc {
//---------------------------------------------------------------------------
void TPCC::LoadCustomer(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        Customer customer;

        // parse TBL file fields
        std::getline(ss, field, '|');
        customer.c_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        customer.c_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        customer.c_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        customer.c_first = imlab::Varchar<16>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_middle = imlab::Char<2>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_last = imlab::Varchar<16>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_street_1 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_street_2 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_city = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_state = imlab::Char<2>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_zip = imlab::Char<9>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_phone = imlab::Char<16>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_since = imlab::Timestamp::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_credit = imlab::Char<2>::build(field.c_str());

        std::getline(ss, field, '|');
        customer.c_credit_lim = imlab::Numeric<12, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_discount = imlab::Numeric<4, 4>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_balance = imlab::Numeric<12, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_ytd_payment = imlab::Numeric<12, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_payment_cnt = imlab::Numeric<4, 0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_delivery_cnt = imlab::Numeric<4, 0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        customer.c_data = imlab::Varchar<500>::build(field.c_str());

        customer_rows.push_back(customer);

        customer_ids[std::make_tuple(customer.c_w_id, customer.c_d_id, customer.c_id)] = imlab::Integer(customer_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadDistrict(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        District district;

        std::getline(ss, field, '|');
        district.d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        district.d_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        district.d_name = imlab::Varchar<10>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_street_1 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_street_2 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_city = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_state = imlab::Char<2>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_zip = imlab::Char<9>::build(field.c_str());

        std::getline(ss, field, '|');
        district.d_tax = imlab::Numeric<4, 4>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        district.d_ytd = imlab::Numeric<12, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        district.d_next_o_id = imlab::Integer(std::stoi(field));

        district_rows.push_back(district);

        district_ids[std::make_tuple(district.d_w_id, district.d_id)] = imlab::Integer(district_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadHistory(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        History history;

        std::getline(ss, field, '|');
        history.h_c_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        history.h_c_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        history.h_c_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        history.h_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        history.h_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        history.h_date = imlab::Timestamp::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        history.h_amount = imlab::Numeric<6, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        history.h_data = imlab::Varchar<24>::build(field.c_str());

        history_rows.push_back(history);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadItem(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        Item item;

        std::getline(ss, field, '|');
        item.i_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        item.i_im_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        item.i_name = imlab::Varchar<24>::build(field.c_str());

        std::getline(ss, field, '|');
        item.i_price = imlab::Numeric<5, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        item.i_data = imlab::Varchar<50>::build(field.c_str());

        item_rows.push_back(item);

        item_ids[item.i_id] = imlab::Integer(item_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadNewOrder(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        NewOrderEntry new_order;

        std::getline(ss, field, '|');
        new_order.no_o_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        new_order.no_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        new_order.no_w_id = imlab::Integer(std::stoi(field));

        neworder_rows.push_back(new_order);

        neworder_ids[std::make_tuple(new_order.no_w_id, new_order.no_d_id, new_order.no_o_id)] = imlab::Integer(neworder_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadOrder(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        Order order;

        std::getline(ss, field, '|');
        order.o_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order.o_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order.o_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order.o_c_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order.o_entry_d = imlab::Timestamp::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        order.o_carrier_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order.o_ol_cnt = imlab::Numeric<2, 0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        order.o_all_local = imlab::Numeric<1, 0>::castString(field.c_str(), field.size());

        order_rows.push_back(order);

        order_ids[std::make_tuple(order.o_w_id, order.o_d_id, order.o_id)]  = imlab::Integer(order_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadOrderLine(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        OrderLine order_line;

        std::getline(ss, field, '|');
        order_line.ol_o_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_d_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_number = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_i_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_supply_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        order_line.ol_delivery_d = imlab::Timestamp::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        order_line.ol_quantity = imlab::Numeric<2, 0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        order_line.ol_amount = imlab::Numeric<6, 2>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        order_line.ol_dist_info = imlab::Char<24>::build(field.c_str());

        orderline_rows.push_back(order_line);

        orderline_ids[std::make_tuple(order_line.ol_w_id, order_line.ol_d_id, order_line.ol_o_id, order_line.ol_number)]  = imlab::Integer(orderline_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadStock(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        Stock stock;

        std::getline(ss, field, '|');
        stock.s_i_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        stock.s_w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        stock.s_quantity = imlab::Numeric<4, 0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        stock.s_dist_01 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_02 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_03 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_04 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_05 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_06 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_07 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_08 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_09 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_dist_10 = imlab::Char<24>::build(field.c_str());

        std::getline(ss, field, '|');
        stock.s_ytd = imlab::Numeric<8,0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        stock.s_order_cnt = imlab::Numeric<4,0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        stock.s_remote_cnt = imlab::Numeric<4,0>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        stock.s_data = imlab::Varchar<50>::build(field.c_str());

        stock_rows.push_back(stock);

        stock_ids[std::make_tuple(stock.s_w_id, stock.s_i_id)]  = imlab::Integer(stock_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadWarehouse(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string field;

        Warehouse warehouse;

        std::getline(ss, field, '|');
        warehouse.w_id = imlab::Integer(std::stoi(field));

        std::getline(ss, field, '|');
        warehouse.w_name = imlab::Varchar<10>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_street_1 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_street_2 = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_city = imlab::Varchar<20>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_state = imlab::Char<2>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_zip = imlab::Char<9>::build(field.c_str());

        std::getline(ss, field, '|');
        warehouse.w_tax = imlab::Numeric<4, 4>::castString(field.c_str(), field.size());

        std::getline(ss, field, '|');
        warehouse.w_ytd = imlab::Numeric<12, 2>::castString(field.c_str(), field.size());

        warehouse_rows.push_back(warehouse);

        warehouse_ids[warehouse.w_id] = imlab::Integer(warehouse_rows.size() - 1);
    }
}
//---------------------------------------------------------------------------
void TPCC::LoadTPCC(const std::string& directory) {
    std::ifstream customer_file(directory + "/tpcc_customer.tbl");
    std::ifstream warehouse_file(directory + "/tpcc_warehouse.tbl");
    std::ifstream district_file(directory + "/tpcc_district.tbl");
    std::ifstream order_file(directory + "/tpcc_order.tbl");
    std::ifstream orderline_file(directory + "/tpcc_orderline.tbl");
    std::ifstream item_file(directory + "/tpcc_item.tbl");
    std::ifstream stock_file(directory + "/tpcc_stock.tbl");
    std::ifstream neworder_file(directory + "/tpcc_neworder.tbl");
    std::ifstream history_file(directory + "/tpcc_history.tbl");

    if (!customer_file.is_open()) {
        std::cerr << "Could not open customer.tbl" << "\n";
        return;
    }
    if (!warehouse_file.is_open()) {
        std::cerr << "Could not open warehouse.tbl" << "\n";
        return;
    }
    if (!district_file.is_open()) {
        std::cerr << "Could not open district.tbl" << "\n";
        return;
    }
    if (!order_file.is_open()) {
        std::cerr << "Could not open order.tbl" << "\n";
        return;
    }
    if (!orderline_file.is_open()) {
        std::cerr << "Could not open orderline.tbl" << "\n";
        return;
    }
    if (!item_file.is_open()) {
        std::cerr << "Could not open item.tbl" << "\n";
        return;
    }
    if (!stock_file.is_open()) {
        std::cerr << "Could not open stock.tbl" << "\n";
        return;
    }
    if (!neworder_file.is_open()) {
        std::cerr << "Could not open neworder.tbl" << "\n";
        return;
    }
    if (!history_file.is_open()) {
        std::cerr << "Could not open history.tbl" << "\n";
        return;
    }

    LoadCustomer(customer_file);
    LoadWarehouse(warehouse_file);
    LoadDistrict(district_file);
    LoadOrder(order_file);
    LoadOrderLine(orderline_file);
    LoadItem(item_file);
    LoadStock(stock_file);
    LoadNewOrder(neworder_file);
    LoadHistory(history_file);
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kCustomer>() {
    return customer_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kDistrict>() {
    return district_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kHistory>() {
    return history_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kItem>() {
    return item_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kNewOrder>() {
    return neworder_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kOrder>() {
    return order_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kOrderLine>() {
    return orderline_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kStock>() {
    return stock_rows.size();
}
//---------------------------------------------------------------------------
template <>
size_t TPCC::Size<statictpcc::kWarehouse>() {
    return warehouse_rows.size();
}
//---------------------------------------------------------------------------

void NewOrder( // we offer 15 different items
    TPCC& data,
    Integer w_id,
    Integer d_id,
    Integer c_id,
    Integer items,
    std::array<Integer, 15>& supware, // by which warehouse the item is supplied
    std::array<Integer, 15>& itemid, // id of each item ordered
    std::array<Integer, 15>& qty, // quantity of each item ordered
    Timestamp datetime) {

    const auto warehouse_iter = data.warehouse_ids.find(w_id);
    if (warehouse_iter == data.warehouse_ids.end() ) {
        std::cerr << "Warehouse id not found" << '\n';
        return;
    }

    size_t warehouse_idx = static_cast<size_t>(warehouse_iter->second.value);
    const auto w_tax = data.warehouse_rows[warehouse_idx].w_tax;

    const auto customer_iter = data.customer_ids.find(std::make_tuple(w_id, d_id, c_id));
    if (customer_iter == data.customer_ids.end()) {
        std::cerr << "Customer id not found" << '\n';
        return;
    }

    size_t customer_idx = static_cast<size_t>(customer_iter->second.value);
    const auto c_discount = data.customer_rows[customer_idx].c_discount;

    const auto district_iter = data.district_ids.find(std::make_tuple(w_id, d_id));
    if (district_iter == data.district_ids.end()) {
        std::cerr << "District id not found" << '\n';
        return;
    }
    size_t district_idx = static_cast<size_t>(district_iter->second.value);
    const auto o_id = data.district_rows[district_idx].d_next_o_id;
    const auto d_tax = data.district_rows[district_idx].d_tax;

    data.district_rows[district_idx].d_next_o_id = imlab::Integer(o_id.value + 1);

    auto all_local = Numeric<1, 0>(imlab::Integer(1));
    for (int i = 0; i < items.value; i++) {
        if (w_id != supware[i]) {
            all_local = Numeric<1, 0>(imlab::Integer(0));
            break;
        }
    }

    data.order_rows.push_back({o_id, d_id, w_id, c_id, datetime, imlab::Integer(0), Numeric<2, 0>(items), all_local});
    data.order_ids[std::make_tuple(w_id, d_id, o_id)] = imlab::Integer(data.order_rows.size() - 1);

    data.neworder_rows.push_back({o_id, d_id, w_id});
    data.neworder_ids[std::make_tuple(w_id, d_id, o_id)] = imlab::Integer(data.neworder_rows.size() - 1);


    for (int i = 0; i < items.value; i++) {
        const auto item_iter = data.item_ids.find(itemid[i]);
        if (item_iter == data.item_ids.end()) {
            std::cerr << "Item id not found" << '\n';
            return;
        }

        size_t item_idx = static_cast<size_t>(item_iter->second.value);
        const auto& i_price = data.item_rows[item_idx].i_price;

        const auto stock_iter_sup = data.stock_ids.find(std::make_tuple(supware[i], itemid[i]));
        if (stock_iter_sup == data.stock_ids.end()) {
            std::cerr << "Stock id not found" << '\n';
            return;
        }

        size_t stock_sup_idx = static_cast<size_t>(stock_iter_sup->second.value);
        auto& stock_row_sup = data.stock_rows[stock_sup_idx];

        auto& s_quantity = stock_row_sup.s_quantity;

        Char<24> s_dist;
        switch (d_id.value) {
            case 1: s_dist = stock_row_sup.s_dist_01; break;
            case 2: s_dist = stock_row_sup.s_dist_02; break;
            case 3: s_dist = stock_row_sup.s_dist_03; break;
            case 4: s_dist = stock_row_sup.s_dist_04; break;
            case 5: s_dist = stock_row_sup.s_dist_05; break;
            case 6: s_dist = stock_row_sup.s_dist_06; break;
            case 7: s_dist = stock_row_sup.s_dist_07; break;
            case 8: s_dist = stock_row_sup.s_dist_08; break;
            case 9: s_dist = stock_row_sup.s_dist_09; break;
            case 10: s_dist = stock_row_sup.s_dist_10; break;
            default:
                std::cerr << "District id doesn't exist" << '\n';
                return;
        }

        if (s_quantity.value > qty[i].value) {
            s_quantity.value -= qty[i].value;
        } else {
            s_quantity.value += 91 - qty[i].value;
        }

        const auto stock_iter_wid = data.stock_ids.find(std::make_tuple(w_id, itemid[i]));
        if (stock_iter_wid == data.stock_ids.end()) {
            std::cerr << "Stock id not found" << '\n';
            return;
        }

        size_t stock_wid_idx = static_cast<size_t>(stock_iter_wid->second.value);
        auto& stock_row_wid = data.stock_rows[stock_wid_idx];

        if (supware[i] != w_id) {
            stock_row_wid.s_remote_cnt = stock_row_wid.s_remote_cnt + Numeric<4, 0>(Integer(1));
        } else {
            stock_row_wid.s_order_cnt = stock_row_wid.s_order_cnt + Numeric<4, 0>(Integer(1));
        }


        auto one = Numeric<6,2>(Integer(1));

        auto qty_6_2 = Numeric<6,2>(qty[i]);

        auto price = i_price.castS<6>();

        auto w_tax_6_2 = w_tax.castM2<4>().castS<6>();
        auto d_tax_6_2 = d_tax.castM2<4>().castS<6>();
        auto c_disc_6_2 = c_discount.castM2<4>().castS<6>();

        auto tax_factor = one + w_tax_6_2 + d_tax_6_2;

        auto discount_factor = one - c_disc_6_2;

        auto partial = (qty_6_2 * price).castM2<6>();

        auto partial2 = (partial * tax_factor).castM2<6>();

        auto ol_amount = (partial2 * discount_factor).castM2<6>();

        data.orderline_rows.push_back({o_id, d_id, w_id, imlab::Integer(i+1), itemid[i], supware[i], Timestamp(0), Numeric<2, 0>(qty[i]), ol_amount, s_dist});
        data.orderline_ids[std::make_tuple(w_id, d_id, o_id, imlab::Integer(i + 1))] = imlab::Integer(data.orderline_rows.size() - 1);
    }
}


















//---------------------------------------------------------------------------
} // namespace statictpcc
//---------------------------------------------------------------------------
