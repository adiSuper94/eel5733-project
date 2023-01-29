cmake_minimum_required(VERSION 3.21)
project(eel5733_project)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

add_executable(email_filter ./src/hw1/email_filter.c ./src/hw1/Filter.h ./src/hw1/Filter.c)
add_executable(calendar_filter ./src/hw1/calendar_filter.c ./src/hw1/Filter.h ./src/hw1/Filter.c)
add_executable(location_updater ./src/hw1/location_updater.c)
