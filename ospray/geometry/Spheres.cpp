// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#undef NDEBUG

// ospray
#include "Spheres.h"
#include "common/Data.h"
#include "common/OSPCommon.h"
#include "common/World.h"
// ispc-generated files
#include "Spheres_ispc.h"

namespace ospray {

  std::string Spheres::toString() const
  {
    return "ospray::Spheres";
  }

  void Spheres::commit()
  {
    radius         = getParam1f("radius", 0.01f);
    bytesPerSphere = getParam1i("bytes_per_sphere", 4 * sizeof(float));
    offset_center  = getParam1i("offset_center", 0);
    offset_radius  = getParam1i("offset_radius", -1);
    sphereData     = getParamData("sphere");
    texcoordData   = getParamData("texcoord");

    if (sphereData.ptr == nullptr) {
      throw std::runtime_error(
          "spheres geometry must have 'sphere' data array");
    }

    numSpheres = sphereData->numBytes / bytesPerSphere;
    postStatusMsg(2) << "#osp: creating 'spheres' geometry, #spheres = "
                     << numSpheres;

    if (numSpheres >= (1ULL << 30)) {
      throw std::runtime_error(
          "too many spheres in this sphere geometry. Consider splitting this "
          "geometry into multiple geometries with fewer spheres. You can still "
          "put all those geometries into a single model, but you can't put "
          "that many spheres into a single geometry without causing address "
          "overflows");
    }

    // check whether we need 64-bit addressing
    huge_mesh = false;
    if (texcoordData && texcoordData->numBytes > INT32_MAX)
      huge_mesh = true;
  }

  size_t Spheres::numPrimitives() const
  {
    return numSpheres;
  }

  LiveGeometry Spheres::createEmbreeGeometry()
  {
    LiveGeometry retval;

    retval.ispcEquivalent = ispc::Spheres_create(this);
    retval.embreeGeometry =
        rtcNewGeometry(ispc_embreeDevice(), RTC_GEOMETRY_TYPE_USER);

    ispc::SpheresGeometry_set(
        retval.ispcEquivalent,
        retval.embreeGeometry,
        sphereData->data,
        texcoordData ? (ispc::vec2f *)texcoordData->data : nullptr,
        numSpheres,
        bytesPerSphere,
        radius,
        offset_center,
        offset_radius,
        huge_mesh);

    return retval;
  }

  OSP_REGISTER_GEOMETRY(Spheres, spheres);

}  // namespace ospray
