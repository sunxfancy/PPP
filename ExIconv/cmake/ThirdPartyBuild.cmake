include(cmake/ExternalProject.cmake)

set (third_party_install_path ${CMAKE_CURRENT_SOURCE_DIR}/extlib)

if (UNIX)

ExternalProject_Add(libiconv
	DOWNLOAD_DIR third_party/
	GIT_REPOSITORY https://github.com/elite-lang/libiconv
	SOURCE_DIR third_party/libiconv/
	CMAKE_COMMAND cmake
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${third_party_install_path}
	# INSTALL_COMMAND ""
	BUILD_ALWAYS 0
	)

elseif(WIN32)

ExternalProject_Add(libiconv
	DOWNLOAD_DIR third_party/
	GIT_REPOSITORY https://github.com/elite-lang/libiconv
	GIT_TAG Windows-x86
	CONFIGURE_COMMAND ""
	SOURCE_DIR third_party/libiconv/
	BUILD_COMMAND "cp_libs.bat"
	BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/third_party/libiconv/"
	INSTALL_COMMAND ""
	)

endif()



ExternalProject_Add(libcharsetdetect
	DOWNLOAD_DIR third_party/
	GIT_REPOSITORY https://github.com/elite-lang/libcharsetdetect
	SOURCE_DIR third_party/libcharsetdetect/
	CMAKE_COMMAND cmake
	CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${third_party_install_path}
	# INSTALL_COMMAND ""
	BUILD_ALWAYS 0
	)


ADD_CUSTOM_TARGET(deps
	DEPENDS libiconv libcharsetdetect
)
