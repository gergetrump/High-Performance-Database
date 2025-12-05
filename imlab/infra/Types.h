//---------------------------------------------------------------------------
// Adapted from:
// HyPer
// (c) Thomas Neumann 2010
//---------------------------------------------------------------------------
#ifndef IMLAB_INFRA_TYPES_H
#define IMLAB_INFRA_TYPES_H
//---------------------------------------------------------------------------
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <ostream>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
// A single type
// Required by test/parser/ParserTest.cc
struct Type {
    // Type class
    enum Class : uint8_t {
        kInteger,
        kTimestamp,
        kNumeric,
        kChar,
        kVarchar,
    };

    // Extract the information of the code
    Class getClass() const { return tclass; }
    uint32_t getPrecision() const { return precision; }
    uint32_t getScale() const { return scale; }
    uint64_t getLength() const { return length; }

    // Static methods to construct a column type
    static Type Integer();
    static Type Timestamp();
    static Type Numeric(unsigned precision, unsigned scale);
    static Type Char(unsigned length);
    static Type Varchar(unsigned length);

    // Equality operator
    bool operator==(Type other);

    private:
    // The type class
    Class tclass;

    // The type argument (if any)
    union {
        // either the information for a numeric
        struct {
            uint32_t precision;
            uint32_t scale;
        };
        // or the length of a (var)char
        uint64_t length;
    };
};
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif // IMLAB_INFRA_TYPES_H
//---------------------------------------------------------------------------
