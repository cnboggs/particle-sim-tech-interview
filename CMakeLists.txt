cmake_minimum_required(VERSION 3.30)
project(ParticleSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simulation
    src/main.cpp
    src/Particle.hpp
    src/ParticleSystem.hpp
    src/Vec3d.hpp
)