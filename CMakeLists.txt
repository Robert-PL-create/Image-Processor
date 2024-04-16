cmake_minimum_required(VERSION 3.27)
project(Image_Processor)

set(CMAKE_CXX_STANDARD 20)

add_executable(Image_Processor
        image_pr.cpp
        parsing/parse.cpp
        parsing/parse.h
        filters/filters.cpp
        filters/filters.h
        image/color.h
        image/image.cpp
        image/image.h
        reading_and_writing/Reader.cpp
        reading_and_writing/Reader.h
        reading_and_writing/Utils.h
        reading_and_writing/Write.cpp
        reading_and_writing/Writer.h)
