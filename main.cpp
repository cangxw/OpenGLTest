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
#include "Texture.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
        100.0f, 100.0f,0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
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
    
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    VertexArray va;
    VertexBuffer vb(position, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2); //顶点位置
    layout.Push<float>(2); //纹理坐标
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
    
    //创建投影矩阵
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100,0,0)); //相机右移一百 = 物体左移一百
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200,200,0)); //物体往右边上边移动200
    glm::mat4 mvp = proj * view * model;
    
    //加载Shader
    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    shader.SetUniformMat4f("u_MVP", mvp);
    
    //绑定texture，并传给shader
    Texture texture("resources/textures/test.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    
    va.UnBind();
    //解绑shader
    shader.Unbind();
    //解绑顶点缓冲区
    vb.Unbind();
    //解绑索引缓冲区
    ib.Unbind();
    
    Renderer renderer;
    
    /**INIT IMGUI START**/
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

#ifdef __APPLE__
  const char* glsl_version = "#version 150";
#endif
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    /**INIT IMGUI END**/
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        
        shader.Bind();
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        
        renderer.Draw(va, ib, shader);
        
        //绘制三角形，起点和数量
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //参数2是索引个数，参数4是数据指针，因为已经绑定了，所以只要填null
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        
        /**RENDER IMGUI START**/
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /**RENDER IMGUI END**/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /**DESTROY IMGUI START**/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    /**DESTROY IMGUI END**/
    
    glfwTerminate();
    return 0;
}
