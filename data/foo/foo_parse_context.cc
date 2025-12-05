// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "data/foo/foo_parse_context.h"
#include "./gen/foo_parser.h"
#include <fmt/format.h>
#include <sstream>
#include <unordered_set>
// ---------------------------------------------------------------------------------------------------
using SchemaParseContext = foo::FooParseContext;
using SomeDeclaration = foo::SomeDeclaration;
using Type = foo::Type;
// ---------------------------------------------------------------------------------------------------
Type Type::Integer() {
    Type t;
    t.tclass = kInteger;
    return t;
}
Type Type::Timestamp() {
    Type t;
    t.tclass = kTimestamp;
    return t;
}
Type Type::Numeric(unsigned length, unsigned precision) {
    Type t;
    t.tclass = kNumeric;
    t.length = length;
    t.precision = precision;
    return t;
}
Type Type::Char(unsigned length) {
    Type t;
    t.tclass = kChar;
    t.length = length;
    return t;
}
Type Type::Varchar(unsigned length) {
    Type t;
    t.tclass = kVarchar;
    t.length = length;
    return t;
}
// ---------------------------------------------------------------------------------------------------
const char* Type::Name() const {
    switch (tclass) {
        case kInteger: return "Integer";
        case kTimestamp: return "Timestamp";
        case kNumeric: return "Numeric";
        case kChar: return "Character";
        case kVarchar: return "Varchar";
        default: return "Unknown";
    }
}
// ---------------------------------------------------------------------------------------------------
// Constructor
SchemaParseContext::FooParseContext(bool trace_scanning, bool trace_parsing)
    : trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
SchemaParseContext::~FooParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
std::vector<std::pair<std::string, std::vector<SomeDeclaration>>>& SchemaParseContext::Parse(std::istream& in) {
    beginScan(in);
    foo::FooParser parser(*this);
    parser.set_debug_level(trace_parsing_);
    parser.parse();
    endScan();
    return ast;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(const std::string& m) {
    throw m;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(uint32_t line, uint32_t column, const std::string& err) {
    std::stringstream s;
    s << "[ l=" << line << " c=" << column << "] " << err;
    throw s.str();
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void SchemaParseContext::defineFoo(const std::string& id, const std::vector<SomeDeclaration>& declarations) {
    ast.push_back({id, declarations});
}
// ---------------------------------------------------------------------------------------------------
