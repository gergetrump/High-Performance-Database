#ifndef CODEGENHELPER_H
#define CODEGENHELPER_H

#include <string>
#include <fstream>
#include <cstdlib>
#include <dlfcn.h>

class CodeGenHelper {
public:
    explicit CodeGenHelper(const std::string& code)
        : code_(code), tempFileName_("/tmp/code.cc"), sharedLibName_("/tmp/code.so"), handle_(nullptr) {}

    void compile() {
        writeToFile();
        std::string compileCommand = "g++ -shared -fPIC -o " + sharedLibName_ + " " + tempFileName_ +
                                " -I/home/parallels/CLionProjects/tasks.cpp" +
                                " -L/home/parallels/CLionProjects/tasks.cpp/cmake-build-debug" +
                                " -limlab";
        int compileResult = system(compileCommand.c_str());
        if (compileResult != 0) {
            throw imlab::RuntimeException("Compilation failed");
        }
    }

    void* loadFunction() {
        handle_ = dlopen(sharedLibName_.c_str(), RTLD_NOW);
        if (!handle_) {
            throw imlab::RuntimeException("Failed to load shared library: " + std::string(dlerror()));
        }
        void* func = dlsym(handle_, "execute");
        if (!func) {
            dlclose(handle_);
            throw imlab::RuntimeException("Failed to load function 'execute': " + std::string(dlerror()));
        }
        return func;
    }


    ~CodeGenHelper() {
        if (handle_) {
            dlclose(handle_);
        }
    }

private:
    void writeToFile() {
        std::ofstream outFile(tempFileName_);
        if (!outFile) {
            throw imlab::RuntimeException("Failed to open file for writing: " + tempFileName_);
        }
        outFile << code_;
        outFile.close();
    }

    std::string code_;
    std::string tempFileName_;
    std::string sharedLibName_;
    void* handle_;
};



#endif //CODEGENHELPER_H
