//---------------------------------------------------------------------------
#include "imlab/Database.h"
#include "imlab/algebra/IU.h"

//---------------------------------------------------------------------------


namespace imlab {

Table::Table(const parser::Table& tableSchema) : tableSchema_(tableSchema) {
    for (size_t i = 0; i < tableSchema_.columns.size(); i++) {
        const auto& column = tableSchema_.columns[i];
        tableColumnIndices_[column.id] = i;
        tableColumnsIUs_.emplace_back(std::make_unique<algebra::IU>(column.id, column.type));
    }
}

void Table::insertRow(const std::vector<Value>& newRow) {
    if (newRow.size() != tableSchema_.columns.size()) {
        throw std::runtime_error("Row size mismatch with column size");
    }
    tableRows_.push_back(newRow);
}

void Database::createTable(const parser::Table& tableSchema) {
    dbTables_.emplace(tableSchema.id, Table(tableSchema));
}

void Database::insertRow(const std::string& tableName, const std::vector<Value>& newRow) {
    auto iter = dbTables_.find(tableName);
    if (iter == dbTables_.end()) {
        throw RuntimeException("Table '" + tableName + "' does not exist");
    }
    iter->second.insertRow(newRow);
}

Table& Database::getTable(const std::string& tableName) {
    auto iter = dbTables_.find(tableName);
    if (iter == dbTables_.end()) {
        throw RuntimeException("Table '" + tableName + "' does not exist");
    }
    return iter->second;
}

bool Database::tableExists(const std::string& tableName) const {
    return dbTables_.contains(tableName);
}

}
