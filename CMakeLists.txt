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



if(WIN32)
    
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)        
    target_link_libraries(EngineMaxim PUBLIC MaxText CookbookWrapperLib glm::glm)   

    add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/Data
                ${CMAKE_CURRENT_BINARY_DIR}/Data)

endif(WIN32)

if(UNIX)

    add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    
    find_package(X11 REQUIRED)

    message(STATUS "X11_FOUND = ${X11_FOUND}")
    message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

    include_directories(${X11_INCLUDE_DIR})

    link_directories(${X11_LIBRARIES})

    # 1. Define Library A
    #add_library(A STATIC a.cpp)
    #target_include_directories(A PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/a_include) # A's public headers

    # 2. Define Library B and link to A
    #add_library(B STATIC b.cpp)
    # B uses A internally, and B's public headers require A's headers.
    # Therefore, B links to A as PUBLIC.
    
    target_link_libraries(CookbookWrapperLib PUBLIC ${X11_LIBRARIES})
    #target_include_directories(B PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/b_include) # B's public headers

    # 3. Define the Executable and link to B
    #add_executable(MyApp main.cpp)
    # MyApp only needs to link to B; A will be pulled in automatically due to transitivity.
    target_link_libraries(EngineMaxim PUBLIC ${X11_LIBRARIES} MaxText CookbookWrapperLib glm::glm)   


    add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/Data
                ${CMAKE_CURRENT_BINARY_DIR}/Data)

endif(UNIX)

target_compile_definitions(EngineMaxim PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)



#target_include_directories(EngineMaxim PRIVATE "./Cookbook" "./Samples/Common_Files" "./Library/Common_Files" "./Library/Source_Files" "./External" "./MaximEngine")