# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(async_command_manager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

#######################################

cmake_minimum_required(VERSION 3.16)

# Получаем имя проекта из имени директории
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} VERSION 1.0.0)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем исходные файлы
file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h include/*.hpp) # Добавляем заголовочные файлы

# Создаем библиотеку
add_library(${PROJECT_NAME} SHARED ${SRC} ${HEADERS}) # Включаем заголовочные файлы в библиотеку
# Добавляем директорию include для заголовочных файлов
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Создаем алиас для библиотеки
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME ON)


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Boost REQUIRED COMPONENTS system serialization)


target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::headers
    Boost::system
    Boost::serialization
)

# Подключаем GoogleTest
# Тесты



file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

# Для каждого .cpp файла создаем отдельный тест
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Извлекаем имя файла без расширения для использования в качестве имени теста
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Создаем исполняемый файл для теста
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Связываем исполняемый файл с библиотеками Google Test и основным проектом
    target_link_libraries(${TEST_NAME} PRIVATE
        "${PROJECT_NAME}::${PROJECT_NAME}"
        gtest_main
        gtest
    )

target_include_directories(${TEST_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# Регистрируем тест
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()


# Указываем правила установки библиотеки
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Установка библиотеки
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Установка исполняемых файлов (если есть)
)

# Устанавливаем заголовочные файлы
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}) # Копируем заголовочные файлы в системную директорию include

# Экспортируем целевой объект для использования в других проектах
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
