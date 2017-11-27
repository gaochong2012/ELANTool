cmake_minimum_required(VERSION 3.8)
project(ELANTool)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp MainWindow.cpp MainWindow.h GCListView.cpp GCListView.h LanIPTable.cpp LanIPTable.h GCArp.cpp GCArp.h QCTools.cpp QCTools.h GCIP.cpp GCIP.h GCSQLite.cpp GCSQLite.h WorkerThread.cpp WorkerThread.h TableRecord.h)
add_executable(ELANTool ${SOURCE_FILES})

#
# Qt5 相关配置
#######################################################################################

#set(CMAKE_AUTOMOC ON)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Sql)
find_package(Qt5Network)

# Tell CMake to create the helloworld executable
#add_executable(helloworld WIN32 main.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(ELANTool Qt5::Widgets Qt5::Sql Qt5::Network )
