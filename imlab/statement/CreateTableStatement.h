//---------------------------------------------------------------------------
#ifndef IMLAB_STATEMENT_CREATETABLESTATEMENT_H
#define IMLAB_STATEMENT_CREATETABLESTATEMENT_H
//---------------------------------------------------------------------------
#include "../../imlab/parser/ParseContext.h"
#include "Statement.h"
#include "imlab/algebra/IU.h"
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
class Database;
//---------------------------------------------------------------------------
namespace statement {
//---------------------------------------------------------------------------
class CreateTableStatement : public Statement {
    public:
    explicit CreateTableStatement(parser::Table table);
    ~CreateTableStatement() override = default;

    void run(Database& db) override;

    const parser::Table& getTable() const { return table; }

    void optimize(imlab::OptimizerPass pass) override {}

    private:
    parser::Table table;
};
//---------------------------------------------------------------------------
} // namespace statement
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_STATEMENT_CREATETABLESTATEMENT_H
//---------------------------------------------------------------------------