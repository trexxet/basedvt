add_custom_target(basedvt_test_all)

function(add_basedvt_test testName)
	add_executable(${testName} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${testName} PRIVATE basedvt)
	target_compile_options(${testName} PRIVATE "-fconcepts-diagnostics-depth=8")
	target_compile_definitions(${testName} PRIVATE
		$<$<CONFIG:Debug>:BASEDVT_DEBUG>
	)
	add_dependencies(basedvt_test_all ${testName})
	add_test(NAME ${testName} COMMAND ${testName})
endfunction()

add_basedvt_test(bv_test_token testToken.cpp)
