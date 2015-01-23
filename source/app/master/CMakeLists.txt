set(the_target MasterSimulator)

project(the_target)

#set(TARGET_REQ_DEPS ${LIB_DEPS})

# need QT libraries 
#find_package(OpenGL REQUIRED)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# finds and sets headers and sources for the module
file(GLOB_RECURSE target_srcs "*.cpp")
file(GLOB_RECURSE target_hdrs "*.h" "*.hpp")
set(qt_hdrs
	MasterDialog.h
	SerialLine.h
	ModbusMaster.h
)

qt4_wrap_cpp(qt_mocs 
	${qt_hdrs}
)

# translator
#set(qt_lang trans_cn.ts)
#qt4_add_translation(qt_translations ${qt_lang})

source_group("Sources" FILES ${target_srcs})
source_group("Headers" FILES ${target_hdrs})
source_group("MOCS" FILES ${qt_mocs})

link_directories(${CMAKE_BINARY_DIR}/libs)

# -------------------------------------------------
# 程序配置文件-程序运行时读取
# -------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/configure/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/configure
	 PATTERN *.*)
	 
add_executable(${the_target} ${target_srcs} ${target_hdrs} ${qt_mocs} ${qt_translations})
target_link_libraries(${the_target} ${QT_LIBRARIES})
#target_link_libraries(${the_target} ${TARGET_REQ_DEPS})

install(TARGETS ${the_target}
  RUNTIME DESTINATION ${BIN_INSTALL_PATH}
  )

set_target_properties(${the_target} PROPERTIES 
  OUTPUT_NAME "${the_target}"
  ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
  INSTALL_NAME_DIR bin
  )
if(MSVC)
	set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


	

