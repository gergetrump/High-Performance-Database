//---------------------------------------------------------------------------
#ifndef IMLAB_STATEMENT_STATEMENT_H
#define IMLAB_STATEMENT_STATEMENT_H
//---------------------------------------------------------------------------
#include "../../imlab/Database.h"
#include "../../imlab/optimizer/OptimizerPass.h"
#include "../../imlab/Database.h"
//---------------------------------------------------------------------------
namespace imlab::statement {
//---------------------------------------------------------------------------
class Statement {
    public:
    enum class Type : uint8_t {
        CreateTableStatementType,
        CopyTableStatementType,
        QueryStatementType,
    };

    explicit Statement(Type type) : type(type) {}
    virtual ~Statement() = default;

    Type getType() const { return type; }

    virtual void run(Database& db) = 0;

    virtual void optimize(imlab::OptimizerPass pass) = 0;

    private:
    Type type;
};
//---------------------------------------------------------------------------
} // namespace imlab::statement
//---------------------------------------------------------------------------
#endif //IMLAB_STATEMENT_STATEMENT_H
//---------------------------------------------------------------------------
