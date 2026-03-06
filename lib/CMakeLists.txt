add_library(basedvt STATIC)

include(cmake/getBasedlib.cmake)

target_link_options(basedvt PRIVATE
	-Wl,--export-all-symbols
)
set_target_properties(basedvt PROPERTIES
	VERSION 0.1
	DEBUG_POSTFIX "dbg"
)
target_compile_definitions(basedvt PRIVATE
	$<$<CONFIG:Debug>:basedvt_DEBUG>
)

set(basedvt_SOURCES
	BasedVT.cpp
)
list(TRANSFORM basedvt_SOURCES PREPEND src/)

target_sources(basedvt PRIVATE ${basedvt_SOURCES})
target_include_directories(basedvt PUBLIC include/)
target_link_libraries(basedvt PUBLIC basedlib)
