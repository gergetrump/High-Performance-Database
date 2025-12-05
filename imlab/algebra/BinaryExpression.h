//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_COMPARE_H
#define IMLAB_ALGEBRA_COMPARE_H
//---------------------------------------------------------------------------
#include "../../imlab/algebra/Expression.h"
#include <memory>
#include <set>
#include <sstream>
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
struct IU;
//---------------------------------------------------------------------------
class BinaryExpression final : public Expression {
    public:
    enum Op : unsigned { Equal, And };

    /// The children
    std::unique_ptr<Expression> left_, right_;
    /// The direction
    Op op_;

    public:
    /// Constructor
    BinaryExpression(Op direction, std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right);
    /// Destructor
    ~BinaryExpression() = default;

    /// Get the direction
    Op getOp() const { return op_; }

    /// Get the type of the expression
    ExpressionType getExpressionType() const override { return Expression::ExpressionType::Binary; }

    /// Collect the ius for the expression
    std::set<const IU*> collectIUs() const override;
    /// Generate codeER
    void produce(std::ostream& out) const override;

    static std::string opToString(Op op) ;

    const Type& getType() const override {
        return left_->getType();
    } // return left type for simplicity

    void assignVarNames(const std::map<const IU*, std::string>& varNames) override;

    std::string toString() const override {
        std::string leftStr = left_->toString();
        std::string rightStr = right_->toString();
        std::string opStr;

        switch (op_) {
            case And: opStr = "AND"; break;
            case Equal: opStr = "="; break;
            default: opStr = "unknown op"; break;
        }

        return "(" + leftStr + " " + opStr + " " + rightStr + ")";
    }

};
//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
#endif //IMLAB_ALGEBRA_COMPARE_H
