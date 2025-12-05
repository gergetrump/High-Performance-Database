//---------------------------------------------------------------------------
#ifndef IMLAB_RUNTIME_RUNTIMEEXCEPTION_H
#define IMLAB_RUNTIME_RUNTIMEEXCEPTION_H
//---------------------------------------------------------------------------
#include <exception>
#include <string>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
class RuntimeException : public std::exception {
    private:
    /// The message that caused the exception
    std::string message;

    public:
    /// Constructor
    explicit RuntimeException(std::string message);
    /// Destructor
    ~RuntimeException() override = default;
    /// Constructor
    RuntimeException(const RuntimeException&) = default;
    /// Constructor
    RuntimeException(RuntimeException&&) noexcept = default;
    /// Assignment
    RuntimeException& operator=(const RuntimeException&) = default;
    /// Assignment
    RuntimeException& operator=(RuntimeException&&) = default;

    /// Get the message
    const std::string& getMessage() const { return message; }

    /// Get the message using the standard exception interface
    const char* what() const noexcept override;

    /// Utility to catch any exception and convert it to a RuntimeMessage
    static RuntimeException ensure(std::exception_ptr exception);
};
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_RUNTIME_RUNTIMEEXCEPTION_H
//---------------------------------------------------------------------------
