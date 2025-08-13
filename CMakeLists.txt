cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(IMGUI_INCLUDE_DIR "./thirdparty/imgui")
set(IMGUI_BACKENDS_INCLUDE_DIR "./thirdparty/imgui/backends")
set(SDL3_INCLUDE_DIR "./thirdparty/SDL/include")
set(GLAD_INCLUDE_DIR "./thirdparty/glad/include")
set(GLM_INCLUDE_DIR "./thirdparty/glm/glm")

#may give problems on linux
if(PRODUCTION_BUILD)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) 
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

if(MSVC) 
add_compile_options(/arch:AVX2) 
endif()

project(TurbidityOfCreatures)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
 
if(BUILD_SHARED_LIBS)
    message(STATUS "${PROJECT_NAME}: Build engine shared library")
    set(BUILD_LIB_FLAG "SHARED")
else()
    message(STATUS "${PROJECT_NAME}: Build engine static library")
    set(BUILD_LIB_FLAG "STATIC")
endif()
add_subdirectory(thirdparty/glad)	
set_target_properties(glad PROPERTIES FOLDER "CMakePredefinedTargets")
add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)
set_target_properties(SDL_uclibc PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(SDL3-static PROPERTIES FOLDER "CMakePredefinedTargets")
add_subdirectory(thirdparty/glm EXCLUDE_FROM_ALL)
set_target_properties(glm PROPERTIES FOLDER "CMakePredefinedTargets")

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

file(GLOB_RECURSE IMGUI_SOURCE_FILES imgui_impl_opengl3.cpp imgui_impl_SDL3.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp)
file(GLOB_RECURSE IMGUI_HEADER_FILES imgui_impl_opengl3.h imgui_impl_SDL3.h imgui.h imconfig.h imgui_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 20)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0) 

else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1) 

endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${MY_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Header Files" FILES ${MY_HEADERS})

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES}  ${MY_HEADERS})

if(MSVC)

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

if (WIN32)
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

message(STATUS "${PROJECT_NAME}: Creating imgui lib")
add_library(imgui ${BUILD_LIB_FLAG} ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
set_target_properties(imgui PROPERTIES FOLDER "CMakePredefinedTargets")

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${SDL3_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${GLAD_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${GLAD_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${GLM_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

if(TARGET SDL3::SDL3main)
    target_link_libraries(TurbidityOfCreatures PUBLIC SDL3::SDL3main)
endif()
target_link_libraries(TurbidityOfCreatures PUBLIC SDL3::SDL3 glad imgui glm::glm)