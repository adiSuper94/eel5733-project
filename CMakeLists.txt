cmake_minimum_required(VERSION 3.21)
project(eel5733_project)

set(CMAKE_C_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB hw_1_lib ./src/hw1/*.h ./src/hw1/filter.c ./src/hw1/calendar.c)
file(GLOB hw_2_lib ./src/hw2/queue.h ./src/hw2/queue.c)
file(GLOB hw_3_lib ./src/hw3/bank.h ./src/hw3/bank.c ./src/hw3/ringbuffer.h ./src/hw3/ringbuffer.c)

add_executable(email_filter ./src/hw1/email_filter.c ${hw_1_lib})
add_executable(calendar_filter ./src/hw1/calendar_filter.c ${hw_1_lib})
add_executable(location_updater ./src/hw1/location_updater.c)

add_executable(threaded_location_updater ./src/hw2/threaded_location_updater.c ${hw_1_lib}
  ${hw_2_lib})
target_link_libraries(threaded_location_updater PRIVATE Threads::Threads)

add_executable(eft ./src/hw3/eft.c ${hw_3_lib})
target_link_libraries(eft PRIVATE Threads::Threads)
