cmake_minimum_required(VERSION 3.22.1)
project(energy_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(gz-plugin2 REQUIRED COMPONENTS register)
find_package(gz-sim8 REQUIRED)

add_library(EnergyPlugin SHARED src/EnergyPlugin.cc)

target_link_libraries(EnergyPlugin
  gz-sim8::gz-sim8
  gz-plugin2::gz-plugin2
)