cmake_minimum_required(VERSION 3.21)
project(eel5733_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
add_executable(filter-emails ./src/hw1/FilterEmails.c ./src/hw1/Filter.h ./src/hw1/Filter.c)
add_executable(parse-calendar-events ./src/hw1/ParseCalendarEvents.c ./src/hw1/Filter.h ./src/hw1/Filter.c)
