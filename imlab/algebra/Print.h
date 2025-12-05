//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_ALGEBRA_PRINT_H
#define IMLAB_ALGEBRA_PRINT_H
//---------------------------------------------------------------------------
#include "../../imlab/algebra/Operator.h"
#include <memory>
#include <vector>
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
class Expression;
//---------------------------------------------------------------------------
class Print final : public UnaryOperator {
    public:
    struct Output {
        std::string name_;
        std::unique_ptr<Expression> expr_;

        Output(std::string name, std::unique_ptr<Expression> expr) : name_(std::move(name)), expr_(std::move(expr)) {}
    };

   /// Constructor
   Print(std::unique_ptr<Operator> child, std::vector<Output> output);
   /// Destructor
   ~Print() override = default;

   /// Collect all IUs produced by the operator
   std::set<const IU*> collectIUs() override;

   /// Prepare the operator
   void prepare(const std::set<const IU*>& required, Operator* consumer) override;
   /// Produce tuple
   void produce(std::ostream& code) override;
   /// Consume tuple
   void consume(std::ostream& code, const Operator* child) override;

   /// Get the operator type
   OperatorType getOperatorType() const override { return OperatorType::Print; }

   /// Perform optimization
   void optimize(std::unique_ptr<Operator>& thisRef, imlab::OptimizerPass pass) override;

   protected:
   /// Output information
   std::vector<Output> output_;


};
//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
#endif // IMLAB_ALGEBRA_PRINT_H
//---------------------------------------------------------------------------
