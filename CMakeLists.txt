cmake_minimum_required(VERSION 3.10)
project(airline)

set(CMAKE_CXX_STANDARD 11)
include_directories(include)

add_executable(airline main.cpp
        include/flight.h src/flight.cpp
        include/plane.h src/plane.cpp
        include/datetime.h src/datetime.cpp
        include/service.h src/service.cpp
        include/ticket.h src/ticket.cpp
        include/luggage.h src/luggage.cpp
        include/handling_car.h src/handling_car.cpp
        include/airport.h src/airport.cpp
        include/customer.h src/customer.cpp
        include/interact.h src/interact.cpp)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs/output")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)
    message("Doxygen build started")
    add_custom_target(Doxygen ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)
