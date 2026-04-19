add_library(basedvt STATIC)

include(cmake/getBasedlib.cmake)

target_link_options(basedvt PRIVATE
	-Wl,--export-all-symbols
)
set_target_properties(basedvt PROPERTIES
	VERSION 0.1
	DEBUG_POSTFIX "dbg"
)
target_compile_definitions(basedvt
	PRIVATE $<$<BOOL:${BASEDVT_ENABLE_LOG}>:BASEDVT_ENABLE_LOG>
	PUBLIC  $<$<BOOL:${BASEDVT_ENABLE_TO_STRING}>:BASEDVT_ENABLE_TO_STRING>
)

set(basedvt_SOURCES
	Decode.cpp
	FSMActions.cpp
	FSMCallbacks.cpp
	FSMClassifier.cpp
	Tokenizer.cpp
)
list(TRANSFORM basedvt_SOURCES PREPEND src/)

target_sources(basedvt PRIVATE ${basedvt_SOURCES})
target_include_directories(basedvt PUBLIC include/)
target_include_directories(basedvt PRIVATE include_internal/)
target_link_libraries(basedvt PUBLIC basedlib)
