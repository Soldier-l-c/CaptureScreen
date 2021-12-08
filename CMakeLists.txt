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

#设置自动生成moc文件，一定要设置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#同理，查找当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./*.ui")
#添加资源文件
set(RCC_FILES rs.qrc)

#查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
FIND_PACKAGE(Qt5 COMPONENTS Widgets Core Gui REQUIRED winextras )


# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
message("message ${DIR_SRCS}")

#通过Ui文件生成对应的头文件，一定要添加
qt5_wrap_ui(WRAP_FILES ${UI_FILES})

# 指定生成目标
add_executable(${PROJECT_NAME} ${DIR_SRCS}  ${WRAP_FILES})

#添加Qt5依赖项
#链接qt 库
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets 
    Qt5::Core 
    Qt5::Gui
	Qt5::WinExtras)

#连接lib须在指定生成目标之后
FIND_PACKAGE(Threads)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})