#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/stacktrace.hpp>

void causeCrash() {
    throw std::runtime_error("Simulated error");
}

void nestedCall() {
    causeCrash();
}

std::uintptr_t get_base_address() {
    std::ifstream maps("/proc/self/maps");
    std::string line;

    while (std::getline(maps, line)) {
        if (line.find("r-xp") != std::string::npos && line.find("sandbox") != std::string::npos) {
            std::istringstream iss(line);
            std::string address_range;
            iss >> address_range;

            size_t dash_pos = address_range.find('-');
            if (dash_pos != std::string::npos) {
                std::string base_str = address_range.substr(0, dash_pos);
                return std::stoull(base_str, nullptr, 16);
            }
        }
    }
    return 0;
}

int main() {
    try {
        throw std::runtime_error("Simulated error");
    }
    catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n";
        std::cerr << "Stack trace:\n";

        std::uintptr_t base = get_base_address();
        for (auto& frame : boost::stacktrace::stacktrace()) {
            auto addr = reinterpret_cast<std::uintptr_t>(frame.address());
            std::uintptr_t offset = addr - base;

            /*std::cerr << "address:" << frame.address()
                << " offset:0x" << std::hex << offset << std::dec
                << "\tname:" << frame.name()
                << "\tsource:" << frame.source_file()
                << ":" << frame.source_line() << "\n";*/
            
            void* ptr = reinterpret_cast<void*>(offset);
            boost::stacktrace::frame f(ptr);
            std::cerr << frame << "\n";
            std::cerr << f << "\n";
        }
    }
    return 0;
}