/*
 * D3D12VertexBufferArray.h
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef LLGL_D3D12_VERTEX_BUFFER_ARRAY_H
#define LLGL_D3D12_VERTEX_BUFFER_ARRAY_H


#include <LLGL/BufferArray.h>
#include <d3d12.h>
#include <vector>


namespace LLGL
{


class Buffer;

class D3D12VertexBufferArray : public BufferArray
{

    public:

        D3D12VertexBufferArray(unsigned int numBuffers, Buffer* const * bufferArray);

        // Returns the array of vertex buffer views.
        inline const std::vector<D3D12_VERTEX_BUFFER_VIEW>& GetViews() const
        {
            return views_;
        }

    private:

        std::vector<D3D12_VERTEX_BUFFER_VIEW> views_;

};


} // /namespace LLGL


#endif



// ================================================================================
