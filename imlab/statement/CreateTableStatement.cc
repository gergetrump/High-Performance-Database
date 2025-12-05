//---------------------------------------------------------------------------
#include "imlab/statement/CreateTableStatement.h"
#include "imlab/Database.h"
#include "../infra/Types.h"
#include <iostream>
#include <mutex>
#include <dlfcn.h>
#include <sstream>
#include <fstream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace imlab::statement {
//---------------------------------------------------------------------------
CreateTableStatement::CreateTableStatement(imlab::parser::Table table) : Statement(Statement::Type::CreateTableStatementType), table(std::move(table)) {}
//---------------------------------------------------------------------------


void CreateTableStatement::run(Database& db) {
    db.createTable(table);
    std::cout << "Table " << table.id << " created." << '\n';
}

//---------------------------------------------------------------------------
} // namespace imlab::statement
//---------------------------------------------------------------------------