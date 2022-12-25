//
//  VertexBuffer.hpp
//  OpenGLTest
//
//  Created by 仓潇伟 on 2022/12/15.
//
#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
