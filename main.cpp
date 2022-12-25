//
//  main.cpp
//  OpenGLTest
//
//  Created by 仓潇伟 on 2022/11/30.
//
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //glfwWindowHint是苹果特殊代码
    //如果不加下面四个glfwWindowHint，在着色器中会报错，提示GLSL不支持version 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //如果使用GLFW_OPENGL_COMPAT_PROFILE，则OpenGL会默认创建一个顶点数组缓存
    //如果使用GLFW_OPENGL_CORE_PROFILE，则需要手动创建一个顶点数组缓存
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #ifdef __APPLE__
    std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif
    
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "oepngl shader version: " << major << "." << minor << std::endl;
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if(glewInit()!=GLEW_OK)
        std::cout<<"Error!"<<std::endl;
    
    std::cout << glGetString(GL_VERSION) << std::endl;
        
    float position[] =
    {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };
    
    unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };
    
    //unsigned int VBO ;

    //因为用了CORE文件，所以需要手动创建一个顶点数组缓存
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
    
    VertexArray va;
    VertexBuffer vb(position, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    
    //告诉GPU顶点缓冲区的数据格式, 这里是顶点buffer和顶点数组buffer实际绑定的地方
    //激活顶点数组缓冲区，索引为0
    //glEnableVertexAttribArray(0);
    //启用顶点属性，指定实际数据的布局
    //告知gpu需要的顶点数据格式
    //第一个参数：属性的index，比如0代表位置，1代表纹理，2代表法线等
    //第二个参数：第三个参数代表的类型的数量，填1,2,3,4；这里指代有两个float，所以填2
    //第四个参数：是否需要归一化
    //第五个参数stride: 每个顶点之间的字节数，如果包含纹理，法线，则需要加上对应的字节
    //第六个参数：属性的偏移字节数，如位置是0，纹理是8，法线是20等，不是0的数字需要用(void *)强制转换
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    //创建绑定索引缓冲区
    IndexBuffer ib(indices, 6);
    
    //加载Shader
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    
    va.UnBind();
    //解绑shader
    shader.Unbind();
    //解绑顶点缓冲区
    vb.Unbind();
    //解绑索引缓冲区
    ib.Unbind();
    
    Renderer renderer;
    
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        
        renderer.Draw(va, ib, shader);
        
        //绘制三角形，起点和数量
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //参数2是索引个数，参数4是数据指针，因为已经绑定了，所以只要填null
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        if(r > 1.0f)
            increment = -0.05f;
        else if(r < 0.0f)
            increment = 0.05f;
        
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
