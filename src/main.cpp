#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/ShaderProgram.h"

int g_windowSizeX = 640;
int g_windowSizeY = 480;

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, // �������
    0.0f, 1.0f, 0.0f, // ������
    0.0f, 0.0f, 1.0f  // �����
};

// in ������� ����������, out ���������� �� �����
const char* vertex_shader =
"#version 460\n" // ������ ������� ������� ����� ������������
"layout(location = 0) in vec3 vertex_postion;" // ������� ������ ������� �������� ��� ������ (������� vertex � color)
"layout(location = 1) in vec3 vertex_color;" // ��� ���������� ��������� ������ ������� ����� ������� ������� ��� �� ������ = layout
"out vec3 color;"
"void main() {"
"   color = vertex_color;" // ������������� �������� �� ����� ��� ���������� ������������
"   gl_Position = vec4(vertex_postion, 1.0);" // ���������� ������� vertex ������� ���� �� ����� � ������������� ����
"}"; // ��� ���� ���������� ����� ����������� ������� ��� ������� � ��� vertex

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);" // ������������� ����������������� �������� ������������ �����
"}";

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int heigth)
{
    g_windowSizeX = width;
    g_windowSizeY = heigth;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void)
{
    // �������������� ����������
    if (!glfwInit()) 
        return -1;

    // �������� ��������� OpenGL (���� ������ ������ ��� 4.6, �� �� �� ������ ������� ����)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ���������� core-profile (������������� ������� OpenGL)

    // ������� ���� � ������� ������
    GLFWwindow* pWindow; pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); // ������� ������������� ���������� ��������� ������� ����
    glfwSetKeyCallback(pWindow, glfwKeyCallback); // ������� ������������� ���������� �������
    
    // ������ ���� �������
    glfwMakeContextCurrent(pWindow);
	
	if (!gladLoadGL())
    {
		std::cerr << "Can't load GLAD" << std::endl;
        return -1;
    }
	
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(0, 1, 1, 1);

    std::string vertexShader(vertex_shader);
    std::string fragemntShader(fragment_shader);
    Renderer::ShaderProgram shaderProgram(vertexShader, fragemntShader);
    if (shaderProgram.isCompiled())
    {
        std::cerr << "Can't create shader program!" << std::endl;
        return -1;
    }

    // �������� ����������� ���������� ��� �������� � ������ ���������� � ������� vbo (vertex buffer obj)
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo); // ������� ��� ������ ���� ������� ������� 1 vbo � ���������� �� ������
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // ���������� ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // ���������� � ���� ������, � ������

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint vao = 0; // VAO �� ������ ���� ������, �� ���������� ����� ����� ��������� � ��������.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // ����������� ������� 0 � �������
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    /*
    0 � ����� �������� (layout).
    3 � �� 3 float�� �� ������� (x, y, z).
    GL_FLOAT � ��� ������.
    GL_FALSE � �� ����� �������������.
    0 � ��� ����������� ����� ��������� (������ ������).
    nullptr � �������� � ������ ������.
    */

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    /*
    VBO ������ ������ ������ �� GPU.
    VAO ������ ���� ��� ������ �������.
    Shader Program ������� �������� layout(0) � layout(1).
    */

    // ������� ����, ���� ���� �� ���������
    while (!glfwWindowShouldClose(pWindow))
    {
        // ������� ���� ����� (��������)
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        GL_TRIANGLES ��� ��� ���������, ������� OpenGL ������ ����������.
        0 ��� ������ ������ �������, � ������� �������� ���������.
        3 ��� ���������� ������, ������� ����� ����������, ������� � first.
        */

        // ������ ������� �������� � ������ �����
        glfwSwapBuffers(pWindow);

        // ������������ ���������������� �������
        glfwPollEvents();
    }

    // ����������� ��������
    glfwTerminate();
    return 0;
}