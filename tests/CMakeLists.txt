add_executable(engelbart_tests
    test_descriptors.cpp
)

target_link_libraries(engelbart_tests PRIVATE engelbart)

add_test(NAME engelbart_tests COMMAND engelbart_tests)
