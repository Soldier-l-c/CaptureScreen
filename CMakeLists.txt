#cmake .
# 生成release
# cmake --build . --config Release  
#32位
# cmake -G "Visual Studio 16 2019" . -A win32

# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (CaptureScreen)


SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -MD -std=c++11 -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11 -pthread")

#生成unicode项目
add_definitions(-DUNICODE -D_UNICODE)

if(MSVC)
#输出变量
message("message $ENV{RAPID_JSON_FOR_LINUX_PIPE_OBJ}")
#增加包含目录
include_directories("$ENV{RAPID_JSON_FOR_LINUX_PIPE_OBJ}")
endif()

include_directories("./Release/uic")

#QT
FIND_PACKAGE(Qt5 COMPONENTS Widgets Core Gui REQUIRED winextras )


# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
message("message ${DIR_SRCS}")
# 指定生成目标,qt项目需要有moc_xxx.cpp,不然会有找不到符号错误
add_executable(CaptureScreen.app ${DIR_SRCS} ./Release/moc/moc_CaptureScreenUI.cpp)

#链接qt 库
target_link_libraries(CaptureScreen.app 
    Qt5::Widgets 
    Qt5::Core 
    Qt5::Gui
	Qt5::WinExtras)

#连接lib须在指定生成目标之后
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(CaptureScreen.app ${CMAKE_THREAD_LIBS_INIT})