cmake_minimum_required(VERSION 3.31)

project(EngineMaxim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(EngineMaxim main.cpp)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	MaxText
	GIT_REPOSITORY	https://github.com/MaxiM863/MaxText.git
    GIT_TAG 	 main
)

FetchContent_MakeAvailable(MaxText)

FetchContent_Declare(
	CookbookWrapperLib
	GIT_REPOSITORY https://github.com/MaxiM863/Cookbook.git
    GIT_TAG main
)

FetchContent_MakeAvailable(CookbookWrapperLib)

FetchContent_Declare(
	OPENAL
	GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG 1.25.1
)

FetchContent_MakeAvailable(OPENAL)

FetchContent_Declare(
	Jolt
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
    GIT_TAG v5.5.0
)

FetchContent_Declare(
	ozz
	GIT_REPOSITORY https://github.com/guillaumeblanc/ozz-animation.git
    GIT_TAG 0.16.0
)

FetchContent_MakeAvailable(ozz)

FetchContent_Declare(
	FreeType
	GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-14-2
)

FetchContent_MakeAvailable(FreeType)

if(WIN32)
    
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)        
    target_link_libraries(EngineMaxim PUBLIC MaxText CookbookWrapperLib glm::glm OPENAL ozz FreeType)

endif(WIN32)

if(UNIX)

    add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    
    find_package(X11 REQUIRED)

    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

    include_directories(${X11_INCLUDE_DIR})

    link_directories(${X11_LIBRARIES})
    
    target_link_libraries(CookbookWrapperLib PUBLIC ${X11_LIBRARIES})
    
    target_link_libraries(EngineMaxim PUBLIC ${X11_LIBRARIES} MaxText CookbookWrapperLib glm::glm OPENAL ozz FreeType)  

endif(UNIX)

target_compile_definitions(EngineMaxim PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_CURRENT_BINARY_DIR}/Data)