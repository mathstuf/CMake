enable_language(C)

add_library(BaseLib OBJECT a.c)
target_compile_definitions(BaseLib INTERFACE REQUIRED)

add_library(UsesLib OBJECT requires.c $<TARGET_OBJECTS:BaseLib>)
target_link_libraries(UsesLib PUBLIC BaseLib)
