cmake_minimum_required(VERSION 3.10)
project(ECommerceInventory CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

add_executable(ecommerce ${SOURCES})

# Make #include "Header.h" work without specifying the path
target_include_directories(ecommerce PRIVATE include)

# Copy the data/ directory into the build folder so the executable can find
# data/products.csv, data/customers.csv, data/orders.csv, data/users.csv
# when run from the build directory.
add_custom_command(TARGET ecommerce POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:ecommerce>/data
    COMMENT "Copying data/ directory to build output"
)
