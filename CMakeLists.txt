cmake_minimum_required(VERSION 3.21)
project(eel5733_project)

set(CMAKE_C_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB hw_1_lib ./src/hw1/*.h ./src/hw1/filter.c ./src/hw1/calendar.c)
add_executable(email_filter ./src/hw1/email_filter.c ${hw_1_lib})
add_executable(calendar_filter ./src/hw1/calendar_filter.c ${hw_1_lib})
add_executable(location_updater ./src/hw1/location_updater.c)

add_executable(threaded_location_updater ./src/hw2/threaded_location_updater.c ${hw_1_lib})
target_link_libraries(threaded_location_updater PRIVATE Threads::Threads)
