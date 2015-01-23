cmake_minimum_required(VERSION 2.8)

# 设置build类型
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configures" FORCE)
#set(CMAKE_BUILD_TYPE Debug CACHE STRING "CMAKE_BUILD_TYPE" FORCE)
if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
	set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

# 设置安装路径
set(BIN_INSTALL_PATH "${CMAKE_BINARY_DIR}/install")
set(CMAKE_INSTALL_PREFIX "${BIN_INSTALL_PATH}")

project(ModbusSimulator CXX C)

option(BUILD_TESTS "Build all tests" ON)

# -------------------------------------------------
# 设置编译和安装路径
# -------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Output directory for application")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin") 

# -------------------------------------------------
# 配置第三方库
# -------------------------------------------------
link_directories(${CMAKE_BINARY_DIR}/libs)

# Need getopt
add_subdirectory(3rdparty/getopt)
set(LIB_DEPS ${LIB_DEPS} getopt)

# Need tinyxml
add_definitions(-DTIXML_USE_STL)
add_subdirectory(3rdparty/tinyxml-2.6.2)
set(LIB_DEPS ${LIB_DEPS} tinyxml)

# -------------------------------------------------
# 配置文件
# -------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/configure/
     DESTINATION ${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/configure
	 PATTERN *.*)
	

# -------------------------------------------------
# 包含目录
# -------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)
#include_directories(${CMAKE_SOURCE_DIR}/module)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/easyloggingpp-8.91)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/getopt)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/tinyxml-2.6.2)
link_directories(${CMAKE_BINARY_DIR}/bin)


#add_subdirectory(module)
add_subdirectory(app/slave)
add_subdirectory(app/master)


# -------------------------------------------------
# 安装
# -------------------------------------------------
#file(GLOB config_files "${CMAKE_SOURCE_DIR}/configure/*.*")
install(FILES ${config_files} DESTINATION "${BIN_INSTALL_PATH}/configure")


################################
# Testing
################################
if (BUILD_TESTS)
	add_subdirectory(3rdparty/gtest-1.7.0)
	#add_subdirectory(tests)
	
endif()