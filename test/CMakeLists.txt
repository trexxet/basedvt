add_custom_target(basedvt_test_all)

function(add_basedvt_test testName)
	add_executable(${testName} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${testName} PRIVATE basedvt)
	target_compile_options(${testName} PRIVATE "-fconcepts-diagnostics-depth=8")
	add_dependencies(basedvt_test_all ${testName})
	add_test(NAME ${testName} COMMAND ${testName})
endfunction()

add_basedvt_test(bvt_test_token_ascii_strict testTokenASCII_Strict.cpp testTokenCommon.cpp)
add_basedvt_test(bvt_test_token_ascii_input testTokenASCII_Input.cpp testTokenCommon.cpp)
add_basedvt_test(bvt_test_decode_ascii testDecodeASCII.cpp)
