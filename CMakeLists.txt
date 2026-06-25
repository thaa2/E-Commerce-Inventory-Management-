cmake_minimum_required(VERSION 3.10)
project(ECommerceInventory CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/AuthManager.cpp
    src/Cart.cpp
    src/Customer.cpp
    src/FileManager.cpp
    src/Inventory.cpp
    src/MenuManager.cpp
    src/Order.cpp
    src/Product.cpp
    src/User.cpp
)

add_executable(ecommerce ${SOURCES})

target_include_directories(ecommerce PRIVATE include)

add_custom_command(TARGET ecommerce POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:ecommerce>/data
    COMMENT "Copying data/ directory to build output"
)
