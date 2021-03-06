/*
 * GLTextureArray.cpp
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLTextureArray.h"
#include "GLTexture.h"
#include "../../../Core/Helper.h"


namespace LLGL
{


GLTextureArray::GLTextureArray(unsigned int numTextures, Texture* const * textureArray)
{
    /* Store the ID of each GLTexture inside the array */
    idArray_.reserve(numTextures);
    targetArray_.reserve(numTextures);

    while (auto next = NextArrayResource<GLTexture>(numTextures, textureArray))
    {
        idArray_.push_back(next->GetID());
        targetArray_.push_back(GLStateManager::GetTextureTarget(next->GetType()));
    }
}


} // /namespace LLGL



// ================================================================================
