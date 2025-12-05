// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef DATA_FOO_FOO_SCHEMA_COMPILER_H_
#define DATA_FOO_FOO_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------
#include "foo_parse_context.h"
#include <ostream>
// ---------------------------------------------------------------------------------------------------
namespace foo {
// ---------------------------------------------------------------------------------------------------
class FooCompiler {
    public:
    // Constructor
    FooCompiler(std::ostream& header, std::ostream& impl)
        : header_(header), impl_(impl) {}
    // Compile a schema
    void Compile(const std::vector<std::pair<std::string, std::vector<SomeDeclaration>>>& schema);

    private:
    // Output stream for the header
    std::ostream& header_;
    // Output stream for the implementation
    std::ostream& impl_;
};
// ---------------------------------------------------------------------------------------------------
} // namespace foo
// ---------------------------------------------------------------------------------------------------
#endif // DATA_FOO_FOO_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------
