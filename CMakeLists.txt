cmake_minimum_required(VERSION 3.15)
project(SandboxProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions (-DBOOST_STACKTRACE_USE_BACKTRACE)

set(CMAKE_C_FLAGS_RELEASE "-O0 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -DNDEBUG -std=c++0x")

# ����� ���������� Boost � ����������� �� ���������
if (WIN32)
    message(STATUS "Detected platform: Windows")
    set(STACKTRACE_COMPONENT "stacktrace_windbg")
else()
    message(STATUS "Detected platform: Linux/Unix")
    set(STACKTRACE_COMPONENT "stacktrace_backtrace")
endif()

# ����� Boost � ������ �����������
find_package(Boost REQUIRED COMPONENTS ${STACKTRACE_COMPONENT})

# �������� ����������� ����
add_executable(sandbox main.cpp)

# ��������� include-���������� � ��������
target_include_directories(sandbox PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(sandbox PRIVATE ${Boost_LIBRARIES})

# Linux: �������� ����� dl
if(UNIX AND NOT APPLE)
    find_library(DL_LIB dl)
    if (DL_LIB)
        target_link_libraries(sandbox PRIVATE ${DL_LIB})
    endif()
endif()
