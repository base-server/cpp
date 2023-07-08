set(BUILD_TARGET "socket-server")
set(ALIAS_NAME "bin::${BUILD_TARGET}")
set(TEST_BUILD_TARGET "${BUILD_TARGET}${POSTFIX_TEST_NAME}")

set(SOURCE
		Main.cpp
		UserChildProcess.cpp
		AdminChildProcess.cpp
	)

set(SOURCE_MAIN
		main.cpp
	)

set(TEST_SOURCE
		test/main.cpp
		test/MainTest.cpp
		test/UserChildProcessTest.cpp
		test/AdminChildProcessTest.cpp
	)

set(DEPENDENCIES
		lib::config
	)

set(CONFIGURE_FILE_TEST
		"test/test.h.in"
	)

func_build_binary(${ALIAS_NAME} ${BUILD_TARGET} "${SOURCE};${SOURCE_MAIN}" "${DEPENDENCIES}")

func_build_unit_test("${TEST_BUILD_TARGET}" "${TEST_SOURCE}" "${SOURCE}" "${DEPENDENCIES}" "${CONFIGURE_FILE_TEST}")
