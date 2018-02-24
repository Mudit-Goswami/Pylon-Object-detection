cmake_minimum_required(VERSION 2.8)
project( detect )
find_package( OpenCV 3.2.0 EXACT REQUIRED )

set(BESLER_INCLUDE_DIRS "/opt/pylon5/include")
set(BESLER_LIBRARY_DIRS "/opt/pylon5/lib64")
set(BESLER_LIBRARIES
 pylonbase
 pylonutility
 GenApi_gcc_v3_0_Basler_pylon_v5_0
 GCBase_gcc_v3_0_Basler_pylon_v5_0
)

include_directories(${BESLER_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${BESLER_LIBRARY_DIRS})
link_directories(${OpenCV_LIB_DIR})
add_executable( detect ravidetect.cpp )
message(STATUS "${OpenCV_LIBRARIES}")
target_link_libraries( detect ${OpenCV_LIBRARIES} )
target_link_libraries(detect ${OpenCV_LIBRARIES})
target_link_libraries(detect ${BESLER_LIBRARIES})

add_executable( Caliberation Caliberation.cpp )
target_link_libraries( Caliberation ${OpenCV_LIBS} )
target_link_libraries( Caliberation ${OpenCV_LIBRARIES} )
target_link_libraries(Caliberation ${OpenCV_LIBRARIES})
target_link_libraries(Caliberation ${BESLER_LIBRARIES})
#add_executable( colour_obj_detection colour_obj_detection.cpp )
#target_link_libraries( colour_obj_detection ${OpenCV_LIBS} )
#add_executable(besler_test "/home/mudit/Desktop/besler-test/besler-test/sources/besler_test.cpp")
#add_executable( camera camera.cpp )
#target_link_libraries( camera ${OpenCV_LIBS} )
