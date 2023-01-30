cmake_minimum_required(VERSION 3.21)
project(eel5733_project)

set(CMAKE_C_STANDARD 17)

file(GLOB hw_1_lib ./src/hw1/*.h ./src/hw1/filter.c ./src/hw1/calendar.c)
add_executable(email_filter ./src/hw1/email_filter.c ${hw_1_lib})
add_executable(calendar_filter ./src/hw1/calendar_filter.c ${hw_1_lib})
add_executable(location_updater ./src/hw1/location_updater.c)
