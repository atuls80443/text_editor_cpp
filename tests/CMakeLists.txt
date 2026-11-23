# Temporary toolchain check. We replace it with real engine tests later.
add_executable(smoke_test smoke_test.cpp)
add_test(NAME smoke_test COMMAND smoke_test)