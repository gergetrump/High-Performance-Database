//---------------------------------------------------------------------------
// IMLAB
//---------------------------------------------------------------------------
#ifndef IMLAB_PARSER_PARSECONTEXT_H
#define IMLAB_PARSER_PARSECONTEXT_H
//---------------------------------------------------------------------------
#include "../../imlab/infra/Types.h"
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
//---------------------------------------------------------------------------
namespace imlab::parser {
//---------------------------------------------------------------------------
class Parser;
class AST;
//---------------------------------------------------------------------------
// Schema parse context
class ParseContext {
    friend Parser;

    public:
    // Constructor
    explicit ParseContext(bool trace_scanning = false, bool trace_parsing = false);
    // Destructor
    virtual ~ParseContext();

    // Parse an istream
    std::vector<std::unique_ptr<AST>> Parse(std::istream& in);

    // Throw an error
    void Error(uint32_t line, uint32_t column, const std::string& err);
    // Throw an error
    void Error(const std::string& m);

    private:
    // Parsed query
    std::vector<std::unique_ptr<AST>> parseTrees;
    // Begin a scan
    void beginScan(std::istream& in);
    // End a scan
    void endScan();

    // Register a parsed AST
    void registerAST(std::vector<std::unique_ptr<AST>>&& trees);

    void registerAST(std::unique_ptr<AST> tree);

    // Trace the scanning
    bool trace_scanning_;
    // Trace the parsing
    bool trace_parsing_;
};
//---------------------------------------------------------------------------
} // namespace imlab::parser
//---------------------------------------------------------------------------
#endif // IMLAB_PARSER_PARSECONTEXT_H
//---------------------------------------------------------------------------
