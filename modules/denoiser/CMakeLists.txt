## ======================================================================== ##
## Copyright 2009-2019 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

option(OSPRAY_MODULE_DENOISER
  "Build the Denoiser FrameOp module for denoising (requires OIDN)" OFF)

if (NOT OSPRAY_MODULE_DENOISER)
  return()
endif()

include(GenerateExportHeader)

add_library(ospray_module_denoiser SHARED DenoiseFrameOp.cpp)

generate_export_header(ospray_module_denoiser)

target_link_libraries(ospray_module_denoiser
PRIVATE
  OpenImageDenoise
  ospray_module_ispc
)

target_include_directories(ospray_module_denoiser
PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)

ospray_install_library(ospray_module_denoiser lib)
