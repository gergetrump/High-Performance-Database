//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_CONST_H
#define IMLAB_ALGEBRA_CONST_H
//---------------------------------------------------------------------------
#include "../../imlab/algebra/Expression.h"
#include <imlab/types/Types.h>
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
class Const final : public Expression {
    public:
    /// Constructor
    explicit Const(std::string constant);

    explicit Const(Value value, Type type);
    /// Destructor
    ~Const() override = default;

    /// Get the expression type
    ExpressionType getExpressionType() const override { return Expression::ExpressionType::Const; }

    /// Collect the ius for the expression
    std::set<const IU*> collectIUs() const override;
    /// Generate code
    void produce(std::ostream& out) const override;

    bool hasType() const { return hasType_; }
    const Type& getType() const override { return type_; }
    const Value& getValue() const { return value_; }
    const std::string& getConstant() const { return constant_; }
    void assignVarNames(const std::map<const IU*, std::string>& varNames) override;

    std::string toString() const override {
        std::ostringstream oss;
        oss << "Const()";
        return oss.str();
    }


    private:
    /// Constant
    std::string constant_;
    Value value_;
    Type type_;
    bool hasType_;
};
//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
#endif //IMLAB_ALGEBRA_CONST_H
