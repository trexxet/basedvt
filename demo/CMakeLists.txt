function(add_basedvt_demo demoName)
	add_executable(${demoName} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${demoName} basedvt)
endfunction()

add_basedvt_demo(basedvt_demo_vt demoVT.cpp)
