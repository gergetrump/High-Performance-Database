//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_IUREF_H
#define IMLAB_ALGEBRA_IUREF_H
//---------------------------------------------------------------------------
#include "IU.h"
#include "../../imlab/algebra/Expression.h"
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
struct IU;
//---------------------------------------------------------------------------
class IURef final : public Expression {
    public:
    /// Constructor
    explicit IURef(const IU* iu);
    /// Destructor
    ~IURef() override = default;

    /// Get the type of the expression
    ExpressionType getExpressionType() const override { return Expression::ExpressionType::IURef; }

    /// Collect the ius for the expression
    std::set<const IU*> collectIUs() const override;
    /// Generate code
    void produce(std::ostream& out) const override;

    const IU* getIU() const {return iu_;}

    const Type& getType() const override { return iu_->type_; }

    void assignVarNames(const std::map<const IU*, std::string>& varNames) override;

    std::string toString() const override {
        std::ostringstream oss;
        if (iu_) {
            oss << "IURef(" << iu_->column_ << ")";
        } else {
            oss << "IURef(NULL)";
        }
        return oss.str();
    }

    private:
    /// The iu
    const IU* iu_;

    std::string varName_;
};
//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
#endif //IMLAB_ALGEBRA_IUREF_H
