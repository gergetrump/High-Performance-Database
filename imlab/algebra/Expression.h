//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_EXPRESSION_H
#define IMLAB_ALGEBRA_EXPRESSION_H
//---------------------------------------------------------------------------
#include "../../imlab/infra/Types.h"

#include <map>
#include <set>
#include <sstream>
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
struct IU;
//---------------------------------------------------------------------------
class Expression {
    public:
    /// Known expression types
    enum class ExpressionType : unsigned {
        Binary,
        Const,
        IURef,
    };

    /// Constructor
    explicit Expression();
    /// Destructor
    virtual ~Expression() = default;

    /// Get the expression type
    virtual ExpressionType getExpressionType() const = 0;

    /// Collect the ius for the expression
    virtual std::set<const IU*> collectIUs() const = 0;
    /// Generate code
    virtual void produce(std::ostream& out) const = 0;

    /// Get a variable name of the expression of type expr_0x0123456789abcd
    virtual std::string getExpressionName() const;

    virtual const Type& getType() const = 0;

    static std::string getAccessorFunction(const Type& type);

    virtual void assignVarNames(const std::map<const IU*, std::string>& varNames) = 0;

    virtual std::string toString() const = 0;

};
//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
#endif //IMLAB_ALGEBRA_EXPRESSION_H
//---------------------------------------------------------------------------
