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
    1.0f, 0.0f, 0.0f, // красный
    0.0f, 1.0f, 0.0f, // зелёный
    0.0f, 0.0f, 1.0f  // синий
};

// in входная переменная, out переменная на выход
const char* vertex_shader =
"#version 460\n" // версия шейдера которую будем использовать
"layout(location = 0) in vec3 vertex_postion;" // входные данные которые получает наш шейдер (позиция vertex и color)
"layout(location = 1) in vec3 vertex_color;" // для правильной установки данных шейдеру нужно указать позицию где их искать = layout
"out vec3 color;"
"void main() {"
"   color = vertex_color;" // устанавливаем значение на выход для дальнейшей интерполяции
"   gl_Position = vec4(vertex_postion, 1.0);" // обозначает позицию vertex который идет на выход в нормированном виде
"}"; // это мини программка будет запускаться столько раз сколько у нас vertex

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);" // устанавливаем интерполированное значение фрагментному цвету
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
    // инициализируем библиотеку
    if (!glfwInit()) 
        return -1;

    // создание контекста OpenGL (если версия меньше чем 4.6, то мы не сможем создать окно)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // используем core-profile (поднмножество функций OpenGL)

    // создаем окно в оконном режиме
    GLFWwindow* pWindow; pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); // функция регистрирущая обработчик изменений размера окна
    glfwSetKeyCallback(pWindow, glfwKeyCallback); // функция регистрирущая обработчик нажатий
    
    // делаем окно текущим
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

    // передача необходимых параметров для шейдеров в память видеокарты с помощью vbo (vertex buffer obj)
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo); // драйвер при вызове этой команды создает 1 vbo и записывает по адресу
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // подключаем его
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // отправляем в него данные, с меткой

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint vao = 0; // VAO не хранит сами данные, он запоминает связи между шейдерами и буферами.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0); // Настраиваем атрибут 0 — позиции
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    /*
    0 — номер атрибута (layout).
    3 — по 3 float’а на вершину (x, y, z).
    GL_FLOAT — тип данных.
    GL_FALSE — не нужно нормализовать.
    0 — нет промежутков между вершинами (строго подряд).
    nullptr — начинаем с начала буфера.
    */

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    /*
    VBO хранят “сырые данные” на GPU.
    VAO хранит “как эти данные читать”.
    Shader Program ожидает атрибуты layout(0) и layout(1).
    */

    // игровой цикл, пока окно не закроется
    while (!glfwWindowShouldClose(pWindow))
    {
        // Очищаем один буфер (рендерим)
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        GL_TRIANGLES Это тип примитива, который OpenGL должен нарисовать.
        0 Это индекс первой вершины, с которой начинать рисование.
        3 Это количество вершин, которые нужно отрисовать, начиная с first.
        */

        // Меняем местами передний и задний буфер
        glfwSwapBuffers(pWindow);

        // Обрабатываем пользовательские команды
        glfwPollEvents();
    }

    // освобожение ресурсов
    glfwTerminate();
    return 0;
}