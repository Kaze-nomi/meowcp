project(MeowCPU)

include_projects_directories(MeowStack)
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
include_projects_directories(MeowCPU)

as_executable()

project_link_projects(MeowStack)

