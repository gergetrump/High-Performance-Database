//---------------------------------------------------------------------------
#include "imlab/algebra/Const.h"
#include <utility>
#include <ostream>
//---------------------------------------------------------------------------
namespace imlab::algebra {
//---------------------------------------------------------------------------
Const::Const(std::string constant) : constant_(std::move(constant)), value_(Integer(0)), type_(), hasType_(false) {}

Const::Const(Value value, Type type) : value_(std::move(value)), type_(std::move(type)), hasType_(true) {}
//---------------------------------------------------------------------------
std::set<const IU*> Const::collectIUs() const {
    return {};
}
//---------------------------------------------------------------------------
void Const::produce(std::ostream& out) const {
    if (hasType_) {
        switch (type_.getClass()) {
            case Type::kInteger: {
                auto value = value_.getInteger();
                out << "imlab::Integer(" << value.value << ")";
                break;
            }
            case Type::kNumeric: {
                unsigned len = type_.getPrecision();
                unsigned precision = type_.getScale();
                int64_t rawValue = value_.getNumericRawValue();
                out << "imlab::Numeric<" << len << ", " << precision << ">::buildRaw(" << rawValue << ")";
                break;
            }
            case Type::kTimestamp: {
                auto timestamp = value_.getTimestamp();
                out << "imlab::Timestamp(" << timestamp.value << ")";
                break;
            }
            case Type::kVarchar:
            case Type::kChar: {
                unsigned maxLength = type_.getLength();
                const std::string& strValue = value_.getStringValue();
                std::string typeName = (type_.getClass() == Type::kVarchar) ? "imlab::Varchar" : "imlab::Char";
                out << typeName << "<" << maxLength << ">::build(\"" << strValue << "\")";
                break;
            }
            default:
                throw RuntimeException("Unsupported type in Const::produce");
        }
    } else {
        throw RuntimeException("Const has no type!");
    }
}

void Const::assignVarNames(const std::map<const IU*, std::string>& varNames) {}


//---------------------------------------------------------------------------
} // namespace imlab::algebra
//---------------------------------------------------------------------------
