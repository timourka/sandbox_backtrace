#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <boost/stacktrace.hpp>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#endif

void causeCrash() {
#ifdef _WIN32
    RaiseException(
        EXCEPTION_ACCESS_VIOLATION,
        0,
        0,
        NULL
    );
#else
    while (1);
#endif
}

void nestedCall() {
    causeCrash();
}


static void signalHandler(int signal)
{
    std::cerr << boost::stacktrace::stacktrace();
}

int main() {
    signal(SIGABRT, &signalHandler);
    signal(SIGTERM, &signalHandler);
    signal(SIGINT, &signalHandler);
    signal(SIGSEGV, &signalHandler);
    nestedCall();
    return 0;
}