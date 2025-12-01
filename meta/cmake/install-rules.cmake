# ---- Dependencies ----

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# aoc binary
install(TARGETS aoc_app RUNTIME COMPONENT aoc_Runtime)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
