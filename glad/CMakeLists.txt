add_library(glad src/glad.c)

target_include_directories(glad
                            INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:include
                            )

target_link_libraries(glad PUBLIC compiler_flags)

set(installable_libs glad compiler_flags)
install (TARGETS ${installable_libs}
          EXPORT gladTargets
          DESTINATION lib)
install (FILES include/glad/glad.h DESTINATION include)
