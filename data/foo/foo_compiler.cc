// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "foo_compiler.h"
#include "foo_parse_context.h"
#include <iostream>
#include <sstream>
// ---------------------------------------------------------------------------------------------------
using FooCompiler = foo::FooCompiler;
using FooParseContext = foo::FooParseContext;
using Type = foo::Type;
// ---------------------------------------------------------------------------------------------------
// Compile a schema
void FooCompiler::Compile(const std::vector<std::pair<std::string, std::vector<SomeDeclaration>>>& schema) {
    for (auto& [id, declarations] : schema) {
        std::cout << "FOO " << id << std::endl;
        for (auto& decl : declarations) {
            std::cout << "    " << decl.id << " " << decl.type.Name() << std::endl;
        }
    }
}
// ---------------------------------------------------------------------------------------------------
