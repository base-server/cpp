set(BUILD_TARGET "socket-server")
set(ALIAS_NAME "bin::${BUILD_TARGET}")
set(TEST_BUILD_TARGET_BASIC "${BUILD_TARGET}${POSTFIX_TEST_NAME}_basic")
set(TEST_BUILD_TARGET_BINARY_STANDALONE "${BUILD_TARGET}${POSTFIX_TEST_NAME}_standalone")
set(TEST_BUILD_TARGET_BINARY_NON_STANDALONE "${BUILD_TARGET}${POSTFIX_TEST_NAME}_non_standalone")

set(SOURCE
		Main.cpp
		UserSocketServerJob.cpp
		AdminSocketServerJob.cpp
		SocketServerChildProcess.cpp
	)

set(SOURCE_MAIN
		main.cpp
	)

set(TEST_SOURCE_BASIC
		test/MainTest.cpp
		test/UserSocketServerJobTest.cpp
		test/AdminSocketServerJobTest.cpp
		test/SocketServerChildProcessTest.cpp
	)

set(TEST_SOURCE_BINARY
		test/userPartOfBinaryTest.cpp
		test/adminPartOfBinaryTest.cpp
	)

set(TEST_SOURCE_MAIN_BASIC
		test/main_basic.cpp
	)

set(TEST_SOURCE_MAIN_BINARY_STANDALONE
		test/main_binary_standalone.cpp
	)

set(TEST_SOURCE_MAIN_BINARY_NON_STANDALONE
		test/main_binary_non_standalone.cpp
	)

set(DEPENDENCIES
		lib::config
	)

set(CONFIGURE_FILE_TEST
		"test/test.h.in"
	)

func_build_binary(${ALIAS_NAME} ${BUILD_TARGET} "${SOURCE};${SOURCE_MAIN}" "${DEPENDENCIES}")

func_build_unit_test("${TEST_BUILD_TARGET_BASIC}" "${TEST_SOURCE_BASIC}" "${SOURCE};${TEST_SOURCE_MAIN_BASIC}" "${DEPENDENCIES}" "${CONFIGURE_FILE_TEST}")
func_build_unit_test("${TEST_BUILD_TARGET_BINARY_STANDALONE}" "${TEST_SOURCE_BINARY}" "${SOURCE};${TEST_SOURCE_MAIN_BINARY_STANDALONE}" "${DEPENDENCIES}" "${CONFIGURE_FILE_TEST}")
func_build_unit_test("${TEST_BUILD_TARGET_BINARY_NON_STANDALONE}" "${TEST_SOURCE_BINARY}" "${SOURCE};${TEST_SOURCE_MAIN_BINARY_NON_STANDALONE}" "${DEPENDENCIES}" "${CONFIGURE_FILE_TEST}")
