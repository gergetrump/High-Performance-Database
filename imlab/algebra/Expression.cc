//---------------------------------------------------------------------------
#include "imlab/algebra/Expression.h"
#include <sstream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
Expression::Expression() = default;
//---------------------------------------------------------------------------
std::string Expression::getExpressionName() const {
    std::stringstream s;
    s << "expr_" << this;
    return s.str();
}

std::string Expression::getAccessorFunction(const Type& type) {
    switch (type.getClass()) {
        case Type::kInteger:
            return "getInteger()";
        case Type::kNumeric:
            return "getNumericRawValue()";
        case Type::kVarchar:
        case Type::kChar:
            return "getStringValue()";
        case Type::kTimestamp:
            return "getTimestamp()";
        default:
            throw std::runtime_error("Unsupported type in getAccessorFunction");
    }
}

//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------