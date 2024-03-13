project(MeowCPU)

include_projects_directories(MeowStack)

as_executable()

project_link_projects(MeowStack)
