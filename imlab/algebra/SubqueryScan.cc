
#include "SubqueryScan.h"

#include "TableScan.h"

namespace imlab::algebra {


SubqueryScan::SubqueryScan(std::unique_ptr<Operator> subquery, const std::string& alias)
    : subquery_(std::move(subquery)), alias_(alias) {}


std::set<const IU*> SubqueryScan::collectIUs() {
    return subquery_->collectIUs();
}

void SubqueryScan::prepare(const std::set<const IU*>& required, Operator* parent) {
    consumer_ = parent;
    requiredIUs_ = required;
    subquery_->prepare(required, this);
}

void SubqueryScan::produce(std::ostream& code) {
    subquery_->produce(code);
}

void SubqueryScan::consume(std::ostream& code, const Operator* child) {
    if (consumer_) {
        consumer_->consume(code, this);
    }
}

Operator::OperatorType SubqueryScan::getOperatorType() const {
    return OperatorType::SubqueryScan;
}

Operator::OperatorCategory SubqueryScan::getOperatorCategory() const {
    return OperatorCategory::Scan;
}

void SubqueryScan::optimize(std::unique_ptr<Operator>& thisRef, OptimizerPass pass) {
    subquery_->optimize(subquery_, pass);
}

const std::string SubqueryScan::getTableName() const {

    if (auto* tableScan = static_cast<const TableScan*>(subquery_.get())) {
        return tableScan->getTableName();
    }
    return "";
}

};



