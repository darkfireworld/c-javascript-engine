MESSAGE(STATUS "Using bundled Findlibmicrohttpd.cmake...")
FIND_PATH(
	LIBMICROHTTPD_INCLUDE_DIR
	microhttpd.h
	/usr/include
	/usr/local/include
)


FIND_LIBRARY(
	LIBMICROHTTPD_LIBRARIES NAMES microhttpd
	PATHS /usr/lib/ /usr/local/lib/
)