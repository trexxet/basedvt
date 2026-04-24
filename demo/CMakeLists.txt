add_custom_target(basedvt_demo_all)

function(add_basedvt_demo demoName)
	add_executable(${demoName} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${demoName} basedvt)
	add_dependencies(basedvt_demo_all ${demoName})
endfunction()

add_basedvt_demo(bvt_demo_input_decode demoInputDecode.cpp demoCommon.cpp)
add_basedvt_demo(bvt_demo_tokenize_strict demoTokenizeStrict.cpp demoCommon.cpp)
