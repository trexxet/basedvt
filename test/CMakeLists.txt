function(add_basedvt_test test_name)
	add_executable(${test_name} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${test_name} basedvt)
endfunction()

add_basedvt_test(basedvt_test_vt testVT.cpp)
