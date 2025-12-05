
#ifndef SUBQUERYSCAN_H
#define SUBQUERYSCAN_H
#include "Operator.h"


#include <memory>

namespace imlab::algebra {

class SubqueryScan final : public Operator {
    std::unique_ptr<Operator> subquery_;
    std::string alias_;

    public:

    SubqueryScan(std::unique_ptr<Operator> subquery, const std::string& alias);

    std::set<const IU*> collectIUs() override;

    void prepare(const std::set<const IU*>& required, Operator* parent) override;

    void produce(std::ostream& code) override;

    void consume(std::ostream& code, const Operator* child) override;

    OperatorType getOperatorType() const override;

    OperatorCategory getOperatorCategory() const override;

    void optimize(std::unique_ptr<Operator>& thisRef, imlab::OptimizerPass pass) override;

    Operator* getSubquery() const {
        return subquery_.get();
    }

    const std::string getTableName() const;

};

}


#endif //SUBQUERYSCAN_H
