//---------------------------------------------------------------------------
#ifndef IMLAB_STATEMENT_COPYTABLESTATEMENT_H
#define IMLAB_STATEMENT_COPYTABLESTATEMENT_H
//---------------------------------------------------------------------------
#include "Statement.h"
#include <imlab/types/Types.h>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
namespace statement {
//---------------------------------------------------------------------------
class CopyTableStatement : public Statement {
    public:
    CopyTableStatement(std::string table, std::string file, std::string delimiter);
    ~CopyTableStatement() override = default;

    void run(Database& db) override;

    std::string_view getTable() const { return table_; }
    std::string_view getFile() const { return file_; }
    std::string_view getDelimiter() const { return delimiter_; }

    void optimize(imlab::OptimizerPass pass) override {}

    private:
    std::string table_;
    std::string file_;
    std::string delimiter_;
};
//---------------------------------------------------------------------------
} // namespace statement
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_STATEMENT_COPYTABLESTATEMENT_H
//---------------------------------------------------------------------------
