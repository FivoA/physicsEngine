# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-src"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-build"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/tmp"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/src/webgpu-backend-wgpu-populate-stamp"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/src"
  "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/src/webgpu-backend-wgpu-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/src/webgpu-backend-wgpu-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/felly/CLionProjects/game-physics-template/cmake-build-debug/_deps/webgpu-backend-wgpu-subbuild/webgpu-backend-wgpu-populate-prefix/src/webgpu-backend-wgpu-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
