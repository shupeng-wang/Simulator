set(the_module tinyxml) 
project(${the_module})

#file(GLOB lib_srcs "*.cpp")
#file(GLOB lib_hdrs "*.h")
set(${the_module}_SOURCES tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp)
set(${the_module}_HEADERS tinyxml.h tinystr.h)
source_group("Sources" FILES ${${the_module}_SOURCES})
source_group("Headers" FILES ${${the_module}_HEADERS})

# dependent libraries for the module
#set(${the_module}_REQ_DEPS ${LIB_DEPS})
#add_definitions(-DTIXML_USE_STL)

#########################################################
# Create the module
#########################################################
add_library(${the_module} STATIC ${${the_module}_HEADERS} ${${the_module}_SOURCES})
#target_link_libraries(${the_module} ${${the_module}_REQ_DEPS})
set(TINYXML_USE_STL ON CACHE BOOL "Build with STL support")
if (TINYXML_USE_STL)
	set_target_properties(${the_module} PROPERTIES COMPILE_DEFINITIONS "TIXML_USE_STL")
endif()

set_target_properties(${the_module} PROPERTIES 
  ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
  INSTALL_NAME_DIR libs
)
  
if(BUILD_SHARED_LIBS)
  if(MSVC)
    set_target_properties(${the_module} PROPERTIES DEFINE_SYMBOL API_EXPORTS)
  else()
    add_definitions(-DAPI_EXPORTS)
  endif()
endif()
  
if(MSVC)
  set_target_properties(${the_module} PROPERTIES 
  LINK_FLAGS "/NODEFAULTLIB:libc /DEBUG")
endif()