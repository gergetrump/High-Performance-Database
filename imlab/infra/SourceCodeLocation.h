//---------------------------------------------------------------------------
// (c) Tobias Schmidt
// Adapted from Umbra
//---------------------------------------------------------------------------
#ifndef IMLAB_INFRA_SOURCECODELOCATION_H
#define IMLAB_INFRA_SOURCECODELOCATION_H
//---------------------------------------------------------------------------
#include <string>
#ifdef __has_include
#if __has_include(<source_location> )
#include <source_location>
#define IMLAB_SOURCELOCATION
#endif
#else
#include <source_location>
#define IMLAB_SOURCELOCATION
#endif
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
#ifdef IMLAB_SOURCELOCATION
//---------------------------------------------------------------------------
/// Source location information
class SourceLocation {
    protected:
    /// The location
    std::source_location debugLocation;

    public:
    /// Constructor
    SourceLocation(std::source_location debugLocation = std::source_location::current()) : debugLocation(debugLocation) {}

    /// Get string representation
    std::string toString() const { return std::string(debugLocation.file_name()) + ':' + std::to_string(debugLocation.line()); }
};
//---------------------------------------------------------------------------
#else
//---------------------------------------------------------------------------
/// Source location information. In optimized builds we do nothing
class SourceLocation {
    public:
    /// Constructor
    SourceLocation() = default;

    /// Get string representation
    std::string toString() const { return "unknown"; }
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
}
#endif //IMLAB_INFRA_SOURCECODELOCATION_H
