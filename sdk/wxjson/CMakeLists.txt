# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)
 
# Our project is called 'wxjson' this is how it will be called in
# visual studio, and in our makefiles. 
project(wxjson)

# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS std REQUIRED)

# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

if(WIN32)
    add_definitions(-DWXMAKINGDLL_WXXS)
    add_definitions(-DWXMAKINGDLL_WXSF)
endif(WIN32)

if ( USE_PCH )
    add_definitions(-include "${CL_PCH_FILE}")
    add_definitions(-Winvalid-pch)
endif ( USE_PCH )


include_directories("${CL_SRC_ROOT}/sdk/wxjson/include" "${CL_SRC_ROOT}/sdk/wxjson/src")

if (UNIX AND NOT APPLE)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
endif()

if ( APPLE )
    add_definitions(-fPIC)
endif()

FILE(GLOB SRCS "src/*.cpp")

# Define the output
add_library(wxjson SHARED ${SRCS})
target_link_libraries(wxjson ${LINKER_OPTIONS} ${wxWidgets_LIBRARIES})

if (NOT MINGW)
    if(APPLE)
        install(TARGETS wxjson DESTINATION ${CMAKE_BINARY_DIR}/codelite.app/Contents/MacOS/)
        CL_INSTALL_NAME_TOOL_STD(${CMAKE_BINARY_DIR}/codelite.app/Contents/MacOS/libwxjson.dylib)
    else()
        install(TARGETS wxjson DESTINATION ${PLUGINS_DIR})
    endif()
else ()
    install(TARGETS wxjson RUNTIME DESTINATION ${CL_PREFIX}/bin LIBRARY DESTINATION ${CL_PREFIX}/lib ARCHIVE DESTINATION ${CL_PREFIX}/lib)
endif()
