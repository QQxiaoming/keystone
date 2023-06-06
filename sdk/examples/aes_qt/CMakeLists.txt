# qt_sdk
set(QT_SDK_DIR "/home/qqm/Downloads/work/keystone/build_qt/host_output")
find_package(Qt5 HINTS "$ENV{QT_SDK_DIR}" COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# aes_lib
set(aes_lib AES/lib/aes.c AES/lib/gmult.c)

# main
set(eapp_bin aes_qt)
set(eapp_src ${aes_lib} eapp/eapp.c)
set(host_bin aes_qt-runner)
set(host_src ${aes_lib} host/main.cpp host/mainwindow.cpp host/mainwindow.cpp host/mainwindow.h host/mainwindow.ui )
set(package_name "aes_qt.ke")
set(package_script "./aes_qt-runner aes_qt eyrie-rt")

if(RISCV32)
  set(eyrie_plugins "rv32 freemem")
else()
  set(eyrie_plugins "freemem")
endif()

# eapp

add_executable(${eapp_bin} ${eapp_src})
target_link_libraries(${eapp_bin} "-nostdlib -static -T ${CMAKE_CURRENT_SOURCE_DIR}/eapp/app.lds" ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})

target_include_directories(${eapp_bin}
  PUBLIC ${KEYSTONE_SDK_DIR}/include/app
  PUBLIC ${KEYSTONE_SDK_DIR}/include/edge)

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})
target_link_libraries ( ${host_bin} Qt5::Widgets )
target_link_libraries ( ${host_bin} Qt5::Core )
target_link_libraries ( ${host_bin} Qt5::Gui )
# add -std=c++11 flag
set_target_properties(${host_bin}
  PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO
)
target_include_directories(${host_bin}
  PUBLIC ${KEYSTONE_SDK_DIR}/include/host
  PUBLIC ${KEYSTONE_SDK_DIR}/include/edge)

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(${eapp_bin}-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(${eapp_bin}-package
  ${package_name}
  ${package_script}
  ${eyrie_files_to_copy} ${eapp_bin} ${host_bin})

add_dependencies(${eapp_bin}-package ${eapp_bin}-eyrie)

# add package to the top-level target
add_dependencies(examples ${eapp_bin}-package)
