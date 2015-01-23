set(the_module getopt) 
project(${the_module})

file(GLOB lib_srcs "*.c")
file(GLOB lib_hdrs "*.h")
set(${the_module}_SOURCES ${lib_srcs})
set(${the_module}_HEADERS ${lib_hdrs})
source_group("Sources" FILES ${${the_module}_SOURCES})
source_group("Headers" FILES ${${the_module}_HEADERS})

# dependent libraries for the module
#set(${the_module}_REQ_DEPS ${LIB_DEPS})

set(BUILD_SHARED_LIBS ON BOOL)

#########################################################
# Create the module
#########################################################
add_library(${the_module} SHARED ${${the_module}_HEADERS} ${${the_module}_SOURCES})
#target_link_libraries(${the_module} ${${the_module}_REQ_DEPS})

set_target_properties(${the_module} PROPERTIES 
  ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
  INSTALL_NAME_DIR libs
)

install(TARGETS getopt 
  RUNTIME DESTINATION ${BIN_INSTALL_PATH}
  )
  
if(BUILD_SHARED_LIBS)
  if(MSVC)
    set_target_properties(${the_module} PROPERTIES DEFINE_SYMBOL EXPORTS_GETOPT)
  else()
    add_definitions(-DEXPORTS_GETOPT)
  endif()
endif()
  
if(MSVC)
  set_target_properties(${the_module} PROPERTIES 
  LINK_FLAGS "/NODEFAULTLIB:libc /DEBUG")
endif()