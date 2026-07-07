execute_process(
    COMMAND
        "${CXX_COMPILER}"
        -std=c++20
        "-I${INCLUDE_DIR}"
        -c
        "${SOURCE}"
        -o
        "${OUTPUT}"
    RESULT_VARIABLE compile_result
    OUTPUT_VARIABLE compile_stdout
    ERROR_VARIABLE compile_stderr
)

set(compile_output "${compile_stdout}\n${compile_stderr}")

if(compile_result EQUAL 0)
    message(FATAL_ERROR "Expected compilation to fail, but it succeeded: ${SOURCE}")
endif()

string(FIND "${compile_output}" "${EXPECTED_FRAGMENT}" fragment_offset)
if(fragment_offset EQUAL -1)
    message(FATAL_ERROR
        "Expected compiler output to contain '${EXPECTED_FRAGMENT}' for ${SOURCE}.\n"
        "Compiler output:\n${compile_output}"
    )
endif()

message(STATUS "Compile failure contained expected fragment: ${EXPECTED_FRAGMENT}")
