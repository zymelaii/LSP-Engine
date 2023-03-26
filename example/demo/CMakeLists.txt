set(PROJECT_NAME demo)

find_package(Qt NAMES Qt6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

aux_source_directory(. SOURCE_FILES)
file(GLOB UI_FILES *.ui)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_FILES})

target_include_directories(
	${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE lspe::lspe
	PRIVATE Qt6::Widgets
)
