cmake_minimum_required(VERSION 3.10)

project(
	libpng
	VERSION 1.6.40
	HOMEPAGE_URL http://www.libpng.org/
	LANGUAGES C
)

add_library(
	${PROJECT_NAME} STATIC
	./png.h
	./pngconf.h
	./pnglibconf.h
	./pngpriv.h
	./pngdebug.h
	./pnginfo.h
	./pngstruct.h
	./png.c
	./pngerror.c
	./pngget.c
	./pngmem.c
	./pngpread.c
	./pngread.c
	./pngrio.c
	./pngrtran.c
	./pngrutil.c
	./pngset.c
	./pngtrans.c
	./pngwio.c
	./pngwrite.c
	./pngwtran.c
	./pngwutil.c
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	../zlib
)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	zlib
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR ANDROID_ABI MATCHES "arm")
    # Disable ARM NEON optimizations explicitly
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        PNG_ARM_NEON_OPT=0
        PNG_ARM_NEON_API_SUPPORTED=0
        PNG_ARM_NEON_CHECK_SUPPORTED=0
    )
    # As an extra precaution, also try to undefine any pre-existing macros
    # though target_compile_definitions should take precedence for values.
    # This might not be strictly necessary if the above are respected.
    target_compile_options(${PROJECT_NAME} PRIVATE
        "-UPNG_ARM_NEON_API_SUPPORTED"
        "-UPNG_ARM_NEON_CHECK_SUPPORTED"
    )
endif()