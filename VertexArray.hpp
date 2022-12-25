//
//  VertexArray.hpp
//  OpenGLTest
//
//  Created by 仓潇伟 on 2022/12/24.
//

#pragma once

#include "VertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    
    void Bind() const;
    void UnBind() const;
};
