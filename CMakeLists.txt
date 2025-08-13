cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AngelSQLite")

add_library (AngelSQLite SHARED "AngelSQLite.cpp"  "sqlite3.def")
target_link_libraries(AngelSQLite PRIVATE sqlite3)
target_link_directories(AngelSQLite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AngelSQLite PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(TARGET AngelSQLite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.dll" "${CMAKE_CURRENT_SOURCE_DIR}/AngelSQLite.as"
    "$<TARGET_FILE_DIR:AngelSQLite>"
)
