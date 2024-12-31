cmake_minimum_required(VERSION 3.13)

project(udisks2-qt6 VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# 查找Qt6组件而不是Qt5
find_package(Qt6 COMPONENTS Core DBus REQUIRED)

# 添加私有头文件
set(PRIVATE_HEADERS
    private/dblockdevice_p.h
)

# 设置源文件
set(SOURCES
    ddiskdevice.cpp
    ddiskmanager.cpp
    udisks2_dbus_common.cpp
    dblockdevice.cpp
    dblockpartition.cpp
    dudisksjob.cpp
)

# 设置头文件
set(HEADERS
    ddiskdevice.h
    udisks2_dbus_common.h
    ddiskmanager.h
    dblockdevice.h
    dblockpartition.h
    dudisksjob.h
    ${PRIVATE_HEADERS}  # 添加私有头文件到头文件列表
)

# 处理DBus接口 (使用Qt6的DBus工具)
set_source_files_properties(org.freedesktop.UDisks2.xml PROPERTIES 
    INCLUDE "udisks2_dbus_common.h"
    NO_NAMESPACE TRUE
)

set_source_files_properties(org.freedesktop.UDisks2.ObjectManager.xml PROPERTIES
    NO_NAMESPACE TRUE
)

qt_add_dbus_interface(DBUS_SOURCES
    org.freedesktop.UDisks2.xml
    udisks2_interface
)

qt_add_dbus_interface(DBUS_SOURCES
    org.freedesktop.UDisks2.ObjectManager.xml
    objectmanager_interface
)

# 创建共享库
add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
    ${DBUS_SOURCES}
)

# 设置SO版本
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME ${PROJECT_NAME}
)

# 设置目标属性
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::DBus
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 安装配置
include(GNUInstallDirs)

# 安装库文件
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# 安装公共头文件（不包括私有头文件）
install(FILES 
    ddiskdevice.h
    udisks2_dbus_common.h
    ddiskmanager.h
    dblockdevice.h
    dblockpartition.h
    dudisksjob.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# 安装private头文件到特定目录
install(FILES ${PRIVATE_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/private
)

# 生成并安装pkgconfig文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/udisks2-qt6.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/udisks2-qt6.pc
    @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/udisks2-qt6.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# 导出目标
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

# 生成cmake配置文件
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 生成版本文件
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 安装cmake配置文件
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
) 