#include <glad/glad.h>
#include <GLFW/glfw3.h> //Библиотека GLFW
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <chrono>

//Исходный код вершинного шейдера
std::string vShader = R"glsl(
    #version 440 core                     //Версия GLSL
    #line 14
    layout(location = 0) in vec2 vPos;    //Координаты вершины примитива
    layout(location = 1) in vec3 vColor;  //Цвет вершины примитива

    uniform mat4 model = mat4(1.0f); //Матрица преобразований

    //Выходные данные вершинного шейдера
    out VS_OUT{
        vec3 outColor;
    }vs_out;

    void main() { 
       vs_out.outColor = vColor;
       gl_Position =  model * vec4(vPos.x, vPos.y, 0.0f, 1.0f); 
    } 

)glsl"
;

//Исходный код пиксельного шейдера
std::string fShader = R"glsl(             
    #version 440 core                    //Версия GLSL
    #line 36

    uniform vec4 color;

    //Входные данные пиксельного шейдера
    in VS_OUT{
        vec3 outColor;
    }fs_in;

    out vec4 fragColor;                  //Выходной цвет пикселя

    void main() { 
       fragColor = vec4(fs_in.outColor.r, fs_in.outColor.g, fs_in.outColor.b, 1.0f); //Изменение значения выходного цвета пикселя
    } 
)glsl"
;

//Проверка компиляции
static void Check(GLuint obj, bool isProgram = false) {
    int status;
    char msg[512];

    if(!isProgram)
        glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    else
        glGetProgramiv(obj, GL_LINK_STATUS, &status);

    if (!status) {
        if (!isProgram)
            glGetShaderInfoLog(obj, 512, 0, msg);
        else
            glGetProgramInfoLog(obj, 512, 0, msg);

        std::cout << msg << std::endl;
    }

}

static GLuint CreateProgram() {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER),
            pixel_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //Конвертируем исходный код из std::string в const char *
    const char* vertex_shader_src   = vShader.c_str();
    const char* fragment_shader_src = fShader.c_str();
    
    glShaderSource(vertex_shader, 1, &vertex_shader_src, 0);
    glCompileShader(vertex_shader);
    Check(vertex_shader); //Проверка компиляции

    glShaderSource(pixel_shader, 1, &fragment_shader_src, 0);
    glCompileShader(pixel_shader);
    Check(pixel_shader); //Проверка компиляции

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, pixel_shader);
    glLinkProgram(program);   
    Check(program, true); //Проверка компоновки

    glDeleteShader(vertex_shader);
    glDeleteShader(pixel_shader);

    return program;
}



GLFWwindow* window;

int main() {
    setlocale(LC_ALL, "rus");

    glfwInit();//Инициализация GLFW

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                //Младшая версия GL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                //Старшая версия GL
                         
                            //Параметры: ширина, высота, название окна
    window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr); //Создание окна
    
    //Позволяет вызывать OpenGL функции для созданного окна
    glfwMakeContextCurrent(window);
 
    //Вертикальная синхронизация
    glfwSwapInterval(1);

    //Загрузка реализаций функций OpenGL из драйверов
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //Массив вершин
    const float vertices[] = {
      //координаты     //цвет
          0.5f,  0.5f, 1, 0, 0, //------------
          0.5f, -0.5f, 0, 1, 0, //Трегольник 1
         -0.5f, -0.5f, 0, 0, 1, //------------
         -0.5f, -0.5f, 1, 0, 0, //------------
         -0.5f,  0.5f, 0, 1, 0, //Треугольник 2
          0.5f,  0.5f, 0, 0, 1, //------------
    };  


    GLuint vao;                         //Массив вершин (Vertex Array Object)
    glGenVertexArrays(1, &vao);         //Сгенерировать идентификатор
    glBindVertexArray(vao);             //Выбрать массив вершин 

    GLuint vbo;                         //Буфер вершин (Vertex Buffer Object)
    glGenBuffers(1, &vbo);              //Сгенерировать идентификатор
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //Выбрать буфер
    //Заполнить буфер данными
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    //Определить интерпретацию считывания из буфера для идентификатора 0 в шейдере
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

    //Определить интерпретацию считывания из буфера для идентификатора 1 в шейдере
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(float) * 2));

    //Разрешить передачу данных по нулевому идентификатору
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint program = CreateProgram();

    //Матрица переноса
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0));
    //Матрица масштабирования
    glm::mat4 scaleMat       = glm::scale(glm::mat4(1.0f),     glm::vec3(0.5, 0.5, 0));
    //Матрица вращения  
    glm::mat4 rotateMat      = glm::rotate(glm::radians(45.f), glm::vec3(0,     0, 1));
    //Матрица преобразований
    glm::mat4 modelMat = translationMat * rotateMat * scaleMat;

    //Время итерации главного цикла
    float deltaTime = 0.0f;
    //Пока окно не закрыто
    while (!glfwWindowShouldClose(window))
    {
        //Время перед выполнением итерации
        auto begin = std::chrono::high_resolution_clock::now();


        glClear(GL_COLOR_BUFFER_BIT);       //Очистить экран
        glClearColor(0.3, 0.3, 0.3, 1.0f);  //Цвет очистки экрана

        //Передача матрицы преобразований в шейдер
        glUseProgram(program); //Выбираем активную программу
        glUniformMatrix4fv(
            glGetUniformLocation(program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelMat)
        );

        glBindVertexArray(vao);             //Выбираем активный массив вершин
        glDrawArrays(GL_TRIANGLES, 0, 6);   //Отрисовать шесть вершин


        if(glfwGetKey(window, GLFW_KEY_W))
            std::cout << "Клавиша нажата" << std::endl;

        //Обработать события ввода
        glfwPollEvents();
        //Чередовать буферы с изображением
        glfwSwapBuffers(window);


        //Время после выполнения итерации
        auto end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f;
    }

    //Освободить ресурсы
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}