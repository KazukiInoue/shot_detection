cmake_minimum_required(VERSION 2.8)



#プロジェクト名の指定

project(shot_detection)



#Debug,Releaseの指定,MinSizeRelとRelWithDebInfoの選択肢を生成しない

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)



#OpenCVのディレクトリの指定

set(OpenCV_DIR "C:/opencv-3.3.0/build")
include_directories("C:/opencv-3.3.0/build/include")

#パッケージの検索
find_package(OpenCV REQUIRED)



#インクルードパスの指定

include_directories( ${OpenCV_INCLUDE_DIRS})
add_executable(shot_detection byBgrThresholdOnly.cpp byBgrThresholdAndColorThemes.cpp inspectThemesConsensus.cpp intValue2Name.cpp judgeByBgrThreshold.cpp main.cpp kMeansColorSubtraction.cpp main.cpp selectPrincipalColorThemes.cpp testExtractedColor.cpp)



#リリース版の設定

set(CMAKE_BUILD_TYPE Debug;Release)




#リンクするライブラリの追加

target_link_libraries(shot_detection ${OpenCV_LIBS})