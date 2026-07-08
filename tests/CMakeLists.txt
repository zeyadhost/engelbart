add_executable(engelbart_tests
    test_descriptors.cpp
)

target_link_libraries(engelbart_tests PRIVATE engelbart)

add_test(NAME engelbart_tests COMMAND engelbart_tests)

function(add_engelbart_compile_failure_test name source expected_fragment)
    add_test(
        NAME ${name}
        COMMAND
            ${CMAKE_COMMAND}
            -D CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -D INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
            -D SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/${source}
            -D OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${name}.o
            -D EXPECTED_FRAGMENT=${expected_fragment}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/expect_compile_failure.cmake
    )
endfunction()

add_engelbart_compile_failure_test(
    engelbart_compile_fail_empty_manufacturer
    compile_fail/invalid_empty_manufacturer.cpp
    "EGB-STR-001"
)

add_engelbart_compile_failure_test(
    engelbart_compile_fail_empty_product
    compile_fail/invalid_empty_product.cpp
    "EGB-VALIDATION"
)

add_engelbart_compile_failure_test(
    engelbart_compile_fail_invalid_vid
    compile_fail/invalid_vid.cpp
    "EGB-VALIDATION"
)

add_engelbart_compile_failure_test(
    engelbart_compile_fail_invalid_pid
    compile_fail/invalid_pid.cpp
    "EGB-VALIDATION"
)

add_engelbart_compile_failure_test(
    engelbart_compile_fail_unsupported_keyboard_nkro
    compile_fail/unsupported_keyboard_nkro.cpp
    "EGB-VALIDATION"
)

add_engelbart_compile_failure_test(
    engelbart_compile_fail_unsupported_mouse_model
    compile_fail/unsupported_mouse_model.cpp
    "EGB-VALIDATION"
)
