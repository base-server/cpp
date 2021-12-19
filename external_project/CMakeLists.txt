add_subdirectory(common-library-cpp)

if(ENABLE_TEST)
	add_subdirectory(googletest)
	add_subdirectory(lcov)
	add_subdirectory(lcov-to-cobertura-xml)
endif(ENABLE_TEST)
