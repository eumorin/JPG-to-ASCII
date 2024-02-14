#======================= Documentation =======================#
include(FindDoxygen)
message("Doxygen build started")
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)
set(DOXYGEN_OUTPUT_LANGUAGE "Russian")
set(DOXYGEN_INPUT_ENCODING "CP1251")
set(DOCUMENTABLE_SOURCES ${CMAKE_SOURCE_DIR}/src)
doxygen_add_docs(doxygen ${DOCUMENTABLE_SOURCES})
#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/src/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

#=================== SETUP EXECTUABLE ====================#

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(JPG-to-ASCII WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(JPG-to-ASCII ${SOURCES})
endif()

# Add the target includes for JPG-to-ASCII
target_include_directories(JPG-to-ASCII PRIVATE ${FORMS_DIR})
target_include_directories(JPG-to-ASCII PRIVATE ${INCLUDE_DIR})
target_include_directories(JPG-to-ASCII PRIVATE ${SOURCE_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries(JPG-to-ASCII PRIVATE Qt6::Widgets)
target_link_libraries(JPG-to-ASCII PRIVATE Qt6::Core)
target_link_libraries(JPG-to-ASCII PRIVATE Qt6::Gui)

#===================== INSTALL SECTION =======================#
install(TARGETS JPG-to-ASCII DESTINATION ${CMAKE_SOURCE_DIR}/bin)

install(FILES $<TARGET_FILE:Qt6::Core>
	DESTINATION bin)
install(FILES $<TARGET_FILE:Qt6::Gui> 
	DESTINATION bin)
install(FILES $<TARGET_FILE:Qt6::Widgets> 
	DESTINATION bin)

install(DIRECTORY "${CMAKE_PREFIX_PATH}/plugins/platforms"
	DESTINATION bin)

