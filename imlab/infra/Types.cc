//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#include "imlab/infra/Types.h"
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
Type Type::Integer() {
    Type t;
    t.tclass = kInteger;
    return t;
}
//---------------------------------------------------------------------------
Type Type::Timestamp() {
    Type t;
    t.tclass = kTimestamp;
    return t;
}
//---------------------------------------------------------------------------
Type Type::Numeric(unsigned precision, unsigned scale) {
    Type t;
    t.tclass = kNumeric;
    t.scale = scale;
    t.precision = precision;
    return t;
}
//---------------------------------------------------------------------------
Type Type::Char(unsigned length) {
    Type t;
    t.tclass = kChar;
    t.length = length;
    return t;
}
//---------------------------------------------------------------------------
Type Type::Varchar(unsigned length) {
    Type t;
    t.tclass = kVarchar;
    t.length = length;
    return t;
}
//---------------------------------------------------------------------------
bool Type::operator==(Type other) {
    if (tclass == kInteger || tclass == kTimestamp) {
        return tclass == other.tclass;
    } else if (tclass == kNumeric) {
        return tclass == other.tclass && precision == other.precision && scale == other.scale;
    } else if (tclass == kChar || tclass == kVarchar) {
        return tclass == other.tclass && length == other.length;
    }
    return false;
}
//---------------------------------------------------------------------------
} // namespace imlab