//---------------------------------------------------------------------------
#include "imlab/algebra/IURef.h"
#include "imlab/algebra/Expression.h"
#include "imlab/algebra/IU.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
IURef::IURef(const IU* iu) : Expression(), iu_(iu) {}
//---------------------------------------------------------------------------
std::set<const IU*> IURef::collectIUs() const {
    return {iu_};
}
//---------------------------------------------------------------------------
void IURef::produce(ostream& out) const {
    out << varName_;
}


void IURef::assignVarNames(const std::map<const IU*, std::string>& varNames) {
    auto it = varNames.find(iu_);
    if (it != varNames.end()) {
        varName_ = it->second;
    } else {
        throw std::runtime_error("Variable name not found for IU: " + iu_->column_);
    }
}


//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------