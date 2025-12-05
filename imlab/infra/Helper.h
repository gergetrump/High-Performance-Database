//---------------------------------------------------------------------------
#ifndef IMLAB_INFRA_HELPER_H
#define IMLAB_INFRA_HELPER_H
//---------------------------------------------------------------------------
#include "imlab/runtime/RuntimeException.h"
#include <algorithm>
#include <cassert>
#include <regex>
#include "imlab/infra/SourceCodeLocation.h"
#include <string>
//---------------------------------------------------------------------------
namespace imlab::helper {
//---------------------------------------------------------------------------
static inline std::string makeSourceCodeLocation(imlab::SourceLocation loc = SourceLocation()) {
    return loc.toString();
}
//---------------------------------------------------------------------------
static inline void tolower(std::string& text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}
//---------------------------------------------------------------------------
static inline std::string removeAdditionalWhitespaces(std::string& text) {
    auto value = std::regex_replace(text, std::regex("^ +| +$|( ) +"), "$1");
    return std::regex_replace(value, std::regex(" +"), " ");
}
//---------------------------------------------------------------------------
[[noreturn]] static inline void notYetImplemented() {
    throw imlab::RuntimeException("not yet implemented");
}
//---------------------------------------------------------------------------
} // namespace helper
//---------------------------------------------------------------------------
#endif //IMLAB_INFRA_HELPER_H
//---------------------------------------------------------------------------
