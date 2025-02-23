cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "yolov8_detect")
project(${PROJECT_NAME})

# 添加预处理器定义
add_definitions(-std=c++17)
add_definitions(-DAPI_EXPORTS)
add_definitions(-DNOMINMAX)
add_definitions(-D_USE_MATH_DEFINES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE release)

# ============= Eigen3 ============
set(Eigen3_INCLUDE_DIRS "D:/Programs/Eigen3/include/eigen3")
include_directories(${Eigen3_INCLUDE_DIRS})

# ============= OpenCV ============
set(OpenCV_DIR "D:/Program/opencv3.4.16/build/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_definitions(-std=c++17)
add_definitions(-DAPI_EXPORTS)

# ============= cuda ============
# include and link dirs of cuda and tensorrt, you need adapt them if yours are different
set(CUDA_TOOLKIT_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8")
find_package(CUDA REQUIRED)
include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)
link_directories(${CUDA_TOOLKIT_ROOT_DIR}/lib64)

set(CMAKE_CUDA_ARCHITECTURES 86)  
set(CMAKE_CUDA_COMPILER ${CUDA_TOOLKIT_ROOT_DIR}/bin/nvcc.exe)
enable_language(CUDA)

# ============= tensorrt ============
set(TENSORRT_DIR "D:/Programs/TensorRT-8.6.1.6")
set(TENSORRT_INCLUDE_DIRS "${TENSORRT_DIR}/include")
set(TENSORRT_LIB_DIRS "${TENSORRT_DIR}/lib")
include_directories(${TENSORRT_INCLUDE_DIRS})
link_directories(${TENSORRT_DIR}/lib)

# ============= plugins ============
add_library(myplugins SHARED ${PROJECT_SOURCE_DIR}/plugin/yololayer.cu)
target_link_libraries(myplugins nvinfer cudart)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/plugin
)
file(GLOB_RECURSE SRCS 
                  ${PROJECT_SOURCE_DIR}/plugin/*.cu
                  ${PROJECT_SOURCE_DIR}/src/*.cpp 
                  ${PROJECT_SOURCE_DIR}/src/*.cu
)
message(${SRCS})
add_executable(yolov8_det 
              ${PROJECT_SOURCE_DIR}/yolov8_det.cpp 
              ${SRCS}
)

target_link_libraries(yolov8_det 
                      ${OpenCV_LIBS}
                      nvinfer
                      nvinfer_plugin
                      cudart
                      nvonnxparser
                      myplugins                      
)

add_executable(yolov8_seg ${PROJECT_SOURCE_DIR}/yolov8_seg.cpp ${SRCS})
target_link_libraries(yolov8_seg nvinfer cudart myplugins ${OpenCV_LIBS})


add_executable(yolov8_pose ${PROJECT_SOURCE_DIR}/yolov8_pose.cpp ${SRCS})
target_link_libraries(yolov8_pose nvinfer cudart myplugins ${OpenCV_LIBS})

add_executable(yolov8_cls ${PROJECT_SOURCE_DIR}/yolov8_cls.cpp ${SRCS})
target_link_libraries(yolov8_cls nvinfer cudart myplugins ${OpenCV_LIBS})

add_executable(yolov8_5u_det ${PROJECT_SOURCE_DIR}/yolov8_5u_det.cpp ${SRCS})
target_link_libraries(yolov8_5u_det nvinfer cudart myplugins ${OpenCV_LIBS})

add_executable(yolov8_obb ${PROJECT_SOURCE_DIR}/yolov8_obb.cpp ${SRCS})
target_link_libraries(yolov8_obb nvinfer cudart myplugins ${OpenCV_LIBS})
