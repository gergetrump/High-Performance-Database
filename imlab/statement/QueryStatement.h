//---------------------------------------------------------------------------
#ifndef IMLAB_STATEMENT_QUERYSTATEMENT_H
#define IMLAB_STATEMENT_QUERYSTATEMENT_H
//---------------------------------------------------------------------------
#include "../../imlab/statement/Statement.h"
#include <memory>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
namespace algebra {
class Operator;
class Expression;
} // namespace algebra
//---------------------------------------------------------------------------
namespace statement {
//---------------------------------------------------------------------------
class QueryStatement : public Statement {
    public:
    explicit QueryStatement(std::unique_ptr<algebra::Operator>);
    ~QueryStatement() override;

    void run(Database& db) override;

    const algebra::Operator* getTree() const { return tree.get(); }

    void optimize(imlab::OptimizerPass pass) override;

    private:
    std::unique_ptr<algebra::Operator> tree;
};
//---------------------------------------------------------------------------
} // namespace statement
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_STATEMENT_QUERYSTATEMENT_H
//---------------------------------------------------------------------------
