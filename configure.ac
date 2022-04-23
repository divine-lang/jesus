AC_INIT(jesus, 1.0, nishiuwu@protonmail.com)
CXXFLAGS="-g -O2 -std=c++17 -Isrc -I/usr/pkg/include -L/usr/pkg/lib"
AC_LANG(C++)
AC_PROG_CXX([c++ g++ clang++])
AC_CHECK_HEADERS([
	iostream
	vector
	string
	curl/curl.h
])

AC_SEARCH_LIBS(curl_global_init, curl, [], [AC_MSG_ERROR(A working libcurl is required)])
CXXFLAGS="${CXXFLAGS} -lcurl"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
