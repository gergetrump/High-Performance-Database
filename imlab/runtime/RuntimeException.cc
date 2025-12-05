//---------------------------------------------------------------------------
#include "RuntimeException.h"
#include <utility>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
RuntimeException::RuntimeException(std::string message) : message(std::move(message)) {}
//---------------------------------------------------------------------------
const char* RuntimeException::what() const noexcept
// Get the message using the standard exception interface
{
    return message.c_str();
}
//---------------------------------------------------------------------------
RuntimeException RuntimeException::ensure(exception_ptr exception)
// Utility to catch any exception and convert them to a RuntimeException
{
    try {
        rethrow_exception(std::move(exception));
    } catch (RuntimeException& e) {
        return std::move(e);
    } catch (const std::exception& e) {
        return RuntimeException(e.what());
    }
}
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------