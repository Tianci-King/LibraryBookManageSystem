cmake_minimum_required(VERSION 3.24)
project(LibraryBookManageSystem)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(LibraryBookManageSystem
        include/book.h
        include/rbTree.h
        include/admin.h
        include/adminMgt.h
        src/book.cpp
        src/main.cpp
        src/admin.cpp
        src/adminMgt.cpp
        src/libraryMgt.cpp
        include/libraryMgt.h
        src/menuPrinter.cpp
        include/menuPrinter.h
        include/fileOperation.h
        src/fileOperation.cpp
)