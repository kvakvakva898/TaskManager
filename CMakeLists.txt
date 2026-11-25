cmake_minimum_required (VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

project (TaskManager LANGUAGES CXX)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable (TaskManager
	src/main.cpp

	src/core/task_service.cpp

	include/gui/main_window.h
	src/gui/main_window.cpp

	include/gui/task_dialog.h
	src/gui/task_dialog.cpp

	include/gui/task_table_model.h
	src/gui/task_table_model.cpp

	src/data/json_task_repository.cpp

	include/gui/import_worker.h
	src/gui/import_worker.cpp)

target_include_directories(TaskManager
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(TaskManager
	PRIVATE
		Qt6::Widgets
		nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TaskManager PROPERTY CXX_STANDARD 20)
endif()

install(TARGETS TaskManager
		BUNDLE DESTINATION .
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
		TARGET TaskManager
		OUTPUT_SCRIPT deploy_script
		NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})