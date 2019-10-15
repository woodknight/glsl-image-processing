/*
Convert YUV NV21 to rgb
*/

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#include <memory>
#include <fstream>
#include <iostream>

// #define __MIAN_DEBUG__

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


const size_t WIDTH = 1600;
const size_t HEIGHT = 720;

int main()
{
    //============= glfw init
    glfwInit();
#ifdef __MAIN_DEBUG__
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "yuv2rgb", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize glad" << std::endl;
        return -1;
    }

    
    //============= create shader

    Shader ourShader("assets/yuv2rgb_vertex.glsl", "assets/yuv2rgb_fragment.glsl");
    ourShader.use();

    float vertices[] = {
        // positions        // texture coords
        -1.0f, -1.0f, 0.0f,      0.0f,    0.0f,
        -1.0f,  1.0f, 0.0f,      0.0f,    1.0f,
         1.0f, -1.0f, 0.0f,      1.0f,    0.0f,
         1.0f,  1.0f, 0.0f,      1.0f,    1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //============= load yuv data
    char *ydata = new char[WIDTH * HEIGHT];
    char *uvdata = new char[WIDTH * HEIGHT / 2];

    std::ifstream yuv_file;
    yuv_file.open("assets/yuv/YUV_Y.bin");
    if(!yuv_file.is_open())
        throw("YUV_Y.bin not found");
    yuv_file.read(ydata, HEIGHT * WIDTH);
    yuv_file.close();

    yuv_file.open("assets/yuv/YUV_U.bin");
    if(!yuv_file.is_open())
        throw("YUV_U.bin not found");
    yuv_file.read(uvdata, HEIGHT * WIDTH / 2);
    yuv_file.close();

    //============= upload yuv data to texture
    GLuint ytex, uvtex, vtex;

    // Y texture
    glGenTextures(1, &ytex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ytex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(ytex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, WIDTH, HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, ydata);

    // UV texture
    glGenTextures(1, &uvtex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, uvtex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(uvtex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, WIDTH / 2, HEIGHT / 2, 0, GL_RG, GL_UNSIGNED_BYTE, uvdata);

    ourShader.setInt("ytex", 0);
    ourShader.setInt("uvtex", 1);

    // render loop
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    delete []ydata;
    delete []uvdata;
    return 0;
}