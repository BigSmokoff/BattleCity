#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int g_windowSizeX = 640;
int g_windowSizeY = 480;

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

    // �������� ��������� openGL (���� ������ ������ ��� 4.6, �� �� �� ������� ������� ����)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // ���������� core-profile (������������� ������� openGL)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ������� ���� � ������� ������
    GLFWwindow* pWindow; pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    
    // ������ ���� �������
    glfwMakeContextCurrent(pWindow);
	
	if (!gladLoadGL())
    {
		std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }
	
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(0, 1, 0, 1);

    // ������� ����, ���� ���� �� ���������
    while (!glfwWindowShouldClose(pWindow))
    {
        // ������� ���� ����� (��������)
        glClear(GL_COLOR_BUFFER_BIT);

        // ������ ������� �������� � ������ �����
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // ����������� ��������
    glfwTerminate();
    return 0;
}