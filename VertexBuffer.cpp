//
//  VertexBuffer.cpp
//  OpenGLTest
//
//  Created by 仓潇伟 on 2022/12/15.
//

#include "VertexBuffer.hpp"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //生成一个buffer缓冲区，buffer得到索引，后面可以用索引得到对应的buffer，因为og是状态机模式，上下文相关
    GLCall(glGenBuffers(1, &m_RendererID));
    //绑定Buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //指定Buffer的数据，数据大小，数据的指针，绘制方式（static，修改一次buffer，多次绘制，dynamic，多次修改多次绘制）
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
