project(MeowStack)

as_interface_library()

enable_testing()


with_test(GTest::gtest_main MeowStack)
include_directories(include)

include(GoogleTest)
gtest_discover_tests(MeowStack_test)