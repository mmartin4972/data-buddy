cmake_minimum_required(VERSION 3.1)

set(project_name data-buddy) ## rename your project here
project(${project_name})
set(CMAKE_CXX_STANDARD 17)

################
#
# OatPP Setup
#
################
add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/Controller.cpp
        src/controller/Controller.hpp
        src/dto/DTOs.hpp
        src/Common.cpp
        src/Common.hpp
        src/controller/RocksWrapper.hpp
)
find_package(oatpp 1.3.0 REQUIRED)
target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
)

#################
#
# RocksDB Setup
#
#################
# Set the library's .so file location
set(RDB_LIBRARY_PATH "/usr/lib/librocksdb.so") # TODO: Make this less janky

# Set the library's include header location
set(RDB_INCLUDE_PATH "/usr/include/rocksdb") # TODO: Make this less janky

# Add the library's .so file as an external library
add_library(rocksdb SHARED IMPORTED)
set_target_properties(rocksdb PROPERTIES IMPORTED_LOCATION ${RDB_LIBRARY_PATH})



find_package(nlohmann_json_schema_validator REQUIRED)
find_package(OpenSSL REQUIRED)

###############
#
# Build Executable
#
###############

target_include_directories(${project_name}-lib PUBLIC src ${RDB_INCLUDE_PATH})

add_executable(${project_name}-exe
        src/App.cpp)

target_link_libraries(${project_name}-exe ${project_name}-lib rocksdb nlohmann_json_schema_validator OpenSSL::SSL OpenSSL::Crypto)

add_dependencies(${project_name}-exe ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe 
        PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)
