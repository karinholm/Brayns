/* Copyright (c) 2015-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * Based on OSPRay implementation
 *
 * This file is part of Brayns <https://github.com/BlueBrain/Brayns>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "AbstractRenderer.h"

// ospray
#include <ospray/SDK/common/Data.h>
#include <ospray/SDK/lights/Light.h>

// sys
#include <vector>

namespace brayns
{
AbstractRenderer::AbstractRenderer()
{
}

void AbstractRenderer::commit()
{
    Renderer::commit();

    _lightData = (ospray::Data*)getParamData("lights");
    _lightArray.clear();

    if (_lightData)
        for (size_t i = 0; i < _lightData->size(); ++i)
            _lightArray.push_back(
                ((ospray::Light**)_lightData->data)[i]->getIE());

    _lightPtr = _lightArray.empty() ? nullptr : &_lightArray[0];

    _shadows = getParam1f("shadows", 0.f);
    _softShadows = getParam1f("softShadows", 0.f);
    _ambientOcclusionStrength = getParam1f("aoWeight", 0.f);
    _ambientOcclusionDistance = getParam1f("aoDistance", 1e20f);
    _shadingEnabled = bool(getParam1i("shadingEnabled", 1));
    _randomNumber = getParam1i("randomNumber", 0);
    _timestamp = getParam1f("timestamp", 0.f);
    _spp = getParam1i("spp", 1);
    _electronShadingEnabled = bool(getParam1i("electronShading", 0));
    _bgMaterial =
        (brayns::obj::ExtendedOBJMaterial*)getParamObject("bgMaterial",
                                                          nullptr);
}

/*! \brief create a material of given type */
ospray::Material* AbstractRenderer::createMaterial(const char*)
{
    ospray::Material* mat = new brayns::obj::ExtendedOBJMaterial;
    return mat;
}
}
