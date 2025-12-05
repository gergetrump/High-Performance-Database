#ifndef IMLAB_INFRA_DEFER_H
#define IMLAB_INFRA_DEFER_H
//---------------------------------------------------------------------------
#include <functional>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------
struct Defer {
   private:
   using FunctionType = std::function<void()>;
   /// The deferred function.
   FunctionType fn;

   public:
   /// Constructor.
   explicit Defer(FunctionType fn)
      : fn(std::move(fn)) {}

   /// Destructor.
   /// Calls the deferred function.
   ~Defer() { fn(); }

   /// Runs the deferred function.
   void run() {
      fn();
      fn = []() {};
   }
};
//---------------------------------------------------------------------------
} // namespace imlab
//---------------------------------------------------------------------------
#endif //IMLAB_INFRA_DEFER_H
//---------------------------------------------------------------------------
