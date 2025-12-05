// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef DATA_FOO_FOO_PARSE_CONTEXT_H_
#define DATA_FOO_FOO_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
// ---------------------------------------------------------------------------------------------------
namespace foo {
// ---------------------------------------------------------------------------------------------------
struct SchemaParser;
struct FooCompiler;
// ---------------------------------------------------------------------------------------------------
// A single type
// Required by test/schemac/schema_parser_test.cc
struct Type {
    // Type class
    enum Class : uint8_t {
        kInteger,
        kTimestamp,
        kNumeric,
        kChar,
        kVarchar,
    };
    // The type class
    Class tclass;
    // The type argument (if any)
    union {
        struct {
            uint32_t length;
            uint32_t precision;
        };
    };

    // Get type name
    const char* Name() const;

    // Static methods to construct a column
    static Type Integer();
    static Type Timestamp();
    static Type Numeric(unsigned length, unsigned precision);
    static Type Char(unsigned length);
    static Type Varchar(unsigned length);
};
// ---------------------------------------------------------------------------------------------------
// Some declaration for the example grammar
struct SomeDeclaration {
    // The identifier
    std::string id;
    // The type
    Type type;

    // Constructor
    explicit SomeDeclaration(const std::string& id = "", Type type = Type::Integer())
        : id(id), type(type) {}
};
// ---------------------------------------------------------------------------------------------------
// Schema parse context
class FooParseContext {
    friend SchemaParser;

    public:
    // Constructor
    explicit FooParseContext(bool trace_scanning = false, bool trace_parsing = false);
    // Destructor
    virtual ~FooParseContext();

    // Parse an istream
    std::vector<std::pair<std::string, std::vector<SomeDeclaration>>>& Parse(std::istream& in);

    // Throw an error
    void Error(uint32_t line, uint32_t column, const std::string& err);
    // Throw an error
    void Error(const std::string& m);

    // Define a table
    void defineFoo(const std::string& id, const std::vector<SomeDeclaration>& declarations);

    private:
    // Begin a scan
    void beginScan(std::istream& in);
    // End a scan
    void endScan();

    // Trace the scanning
    bool trace_scanning_;
    // Trace the parsing
    bool trace_parsing_;

    // a vector of the parsed statements
    std::vector<std::pair<std::string, std::vector<SomeDeclaration>>> ast;
};
// ---------------------------------------------------------------------------------------------------
} // namespace foo
// ---------------------------------------------------------------------------------------------------
#endif // DATA_FOO_FOO_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------
