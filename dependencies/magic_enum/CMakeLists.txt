# Adapted third from TwinStar ToolKit Source

project(
	magic_enum
	VERSION 0.9.6
	HOMEPAGE_URL https://github.com/Neargye/magic_enum
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} INTERFACE
)

target_include_directories(
	${PROJECT_NAME} INTERFACE .
	../fmt
)

target_link_libraries(
	${PROJECT_NAME} INTERFACE
	fmt
)

# Set Kind

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# UTF8 Support
if(MSVC)
  target_compile_options(${PROJECT_NAME} INTERFACE /utf-8)
  target_compile_options(${PROJECT_NAME} INTERFACE "$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  target_compile_options(${PROJECT_NAME} INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
  target_compile_options(${PROJECT_NAME} INTERFACE -finput-charset=UTF-8)
endif()
