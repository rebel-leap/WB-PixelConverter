cmake_minimum_required(VERSION 3.16)
project(WBConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(WBConverter SHARED
    src/WBConverter.cpp
    src/AbsorptionModel.cpp
)

target_include_directories(WBConverter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    set_target_properties(WBConverter PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

include(GNUInstallDirs)
install(TARGETS WBConverter
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/WBConverter)