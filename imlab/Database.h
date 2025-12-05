//---------------------------------------------------------------------------
#ifndef IMLAB_DATABASE_H
#define IMLAB_DATABASE_H
//---------------------------------------------------------------------------
#include "../imlab/parser/AST.h"
#include "../imlab/parser/ParseContext.h"
#include "../imlab/types/Types.h"
#include "../imlab/algebra/IU.h"

//---------------------------------------------------------------------------
namespace imlab {namespace algebra {
class IU;}
//---------------------------------------------------------------------------
namespace semana {
class SemanticalAnalysis;
} // namespace semana
//---------------------------------------------------------------------------
class Table {

    parser::Table tableSchema_;

    std::vector<std::vector<Value>> tableRows_;
    std::unordered_map<std::string, size_t> tableColumnIndices_;

    std::vector<std::unique_ptr<algebra::IU>> tableColumnsIUs_;

public:

    explicit Table(const parser::Table& tableSchema);

    void insertRow(const std::vector<Value>& newRow);

    const parser::Table& getSchema() const {
      return tableSchema_;
    }

    const std::unordered_map<std::string, size_t>& getColumnIndices() const {
      return tableColumnIndices_;
    }

    const std::vector<std::vector<Value>>& getRows() const {
      return tableRows_;
    }

    const std::vector<std::unique_ptr<algebra::IU>>& getColumnsIUs() const {
      return tableColumnsIUs_;
    }
};
//---------------------------------------------------------------------------
class Database {
    friend semana::SemanticalAnalysis;

    /// the tables
    std::unordered_map<std::string, Table> dbTables_;

public:

    Database() = default;

    void createTable(const parser::Table& tableSchema);

    void insertRow(const std::string& tableName, const std::vector<Value>& newRow);

    Table& getTable(const std::string& tableName);

    bool tableExists(const std::string& tableName) const;

};
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_DATABASE_H
