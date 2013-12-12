add_executable(corbasim_idl corbasim_idl.cpp generator.cpp)
target_link_libraries(corbasim_idl idl_parser idlmm)
