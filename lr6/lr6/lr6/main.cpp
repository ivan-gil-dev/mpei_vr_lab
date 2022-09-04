#include <glad/glad.h>
#include <GLFW/glfw3.h> //Библиотека GLFW
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <array>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#define USE_GOURAUD
//#define USE_ORTHO

//Массив вершин плоскости
std::array<float, 11 * 6> CreatePlaneArray() {
    static float uv_max = 1.0f;
    static float uv_min = 0.0f;

    //Массив вершин
    static std::array<float, 11 * 6> vertices = {
            //координаты       цвет     UV              нормаль
            0.5f,  0.5f, 0.0f, 1, 0, 0, uv_max, uv_max, 0, 0, 1, //------------
            0.5f, -0.5f, 0.0f, 0, 1, 0, uv_max, uv_min, 0, 0, 1, //Трегольник 1
           -0.5f, -0.5f, 0.0f, 0, 0, 1, uv_min, uv_min, 0, 0, 1, //------------
           -0.5f, -0.5f, 0.0f, 0, 0, 0, uv_min, uv_min, 0, 0, 1, //------------
           -0.5f,  0.5f, 0.0f, 0, 1, 0, uv_min, uv_max, 0, 0, 1, //Треугольник 2
            0.5f,  0.5f, 0.0f, 0, 0, 1, uv_max, uv_max, 0, 0, 1 //------------
    };

    return vertices;
}

//Массив вершин куба
std::array<float, 11 * 36> CreateCubeArray() {
    //Массив вершин
    static std::array<float, 11 * 36> vertices = {
        //координаты         цвет                UV         Нормаль
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
    };

    return vertices;
}

//Массив вершин пирамиды
std::array<float, 11 * 18> CreatePyramidArray() {

    glm::vec3 normal1;
    {
        glm::vec3 AB = glm::vec3(0, 0, 0.5f) - glm::vec3(-0.5f, 0.5f, -0.5f);
        glm::vec3 AC = glm::vec3(0, 0, 0.5f) - glm::vec3(-0.5f, -0.5f, -0.5f);
        normal1 = glm::cross(AB, AC);
    }

    glm::vec3 normal2;
    {
        glm::vec3 AB = glm::vec3(0, 0, 0.5f) - glm::vec3(-0.5f, -0.5f, -0.5f);
        glm::vec3 AC = glm::vec3(0, 0, 0.5f) - glm::vec3( 0.5f, -0.5f, -0.5f);
        normal2 = glm::cross(AB, AC);
    }

    glm::vec3 normal3;
    {
        glm::vec3 AB = glm::vec3(0, 0, 0.5f) - glm::vec3(0.5f, -0.5f, -0.5f);
        glm::vec3 AC = glm::vec3(0, 0, 0.5f) - glm::vec3(0.5f, 0.5f, -0.5f);
        normal3 = glm::cross(AB, AC);
    }

    glm::vec3 normal4;
    {
        glm::vec3 AB = glm::vec3(0, 0, 0.5f) - glm::vec3(0.5f, 0.5f, -0.5f);
        glm::vec3 AC = glm::vec3(0, 0, 0.5f) - glm::vec3(-0.5f, 0.5f, -0.5f);
        normal4 = glm::cross(AB, AC);
    }

    //Массив вершин
    static std::array<float, 11 * 18> vertices = {
        //координаты        цвет              UV           Нормаль
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  normal1.x, normal1.y, normal1.z,
        0,     0,     0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  normal1.x, normal1.y, normal1.z,
       -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  normal1.x, normal1.y, normal1.z,
                                                          
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, normal2.x, normal2.y, normal2.z,
        0,     0,     0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  normal2.x, normal2.y, normal2.z,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  normal2.x, normal2.y, normal2.z,
                                                         
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  normal3.x, normal3.y, normal3.z,
        0,     0,     0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  normal3.x, normal3.y, normal3.z,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  normal3.x, normal3.y, normal3.z,
                                                        
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  normal4.x, normal4.y, normal4.z,
        0,     0,     0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  normal4.x, normal4.y, normal4.z,
       -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  normal4.x, normal4.y, normal4.z,

        0.5f,  0.5f, -0.5f, 1,    0,    0,     1,    1,    0, 0, -1,
        0.5f, -0.5f, -0.5f, 0,    1,    0,     1,    0,    0, 0, -1,
        -0.5f, -0.5f,-0.5f, 0,    0,    1,     0,    0,    0, 0, -1,
        -0.5f, -0.5f,-0.5f, 0,    0,    0,     0,    0,    0, 0, -1,
        -0.5f,  0.5f,-0.5f, 0,    1,    0,     0,    1,    0, 0, -1,
        0.5f,  0.5f, -0.5f, 0,    0,    1,     1,    1,    0, 0, -1
    };

    return vertices;
}


void SetTransform(GLuint program, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    //Матрица переноса
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
    //Матрица масштабирования
    glm::mat4 scaleMat       = glm::scale(glm::mat4(1.0f), scale);
    //Матрица вращения  
    glm::mat4 rotateMatX     = glm::rotate(glm::radians(rotation.x), glm::vec3(1, 0, 1));
    //Матрица вращения  
    glm::mat4 rotateMatY     = glm::rotate(glm::radians(rotation.y), glm::vec3(0, 1, 0));
    //Матрица вращения  
    glm::mat4 rotateMatZ     = glm::rotate(glm::radians(rotation.z), glm::vec3(0, 0, 1));
    //Матрица преобразований
    glm::mat4 modelMat = translationMat * (rotateMatX * rotateMatY * rotateMatZ) * scaleMat;

    glUseProgram(program);//Выбираем активную программу

    //Передача матрицы преобразований в шейдер
    glUniformMatrix4fv(
        glGetUniformLocation(program, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(modelMat)
    );

}

#ifndef USE_GOURAUD
//Исходный код вершинного шейдера
std::string vShader = R"glsl(
    #version 440 core                     //Версия GLSL
    #line 16
    layout(location = 0) in vec3 vPos;    //Координаты вершины примитива
    layout(location = 1) in vec3 vColor;  //Цвет вершины примитива
    layout(location = 2) in vec2 vUV;     //UV координаты 
    layout(location = 3) in vec3 vNormal; //Нормаль 

    uniform mat4 projection = mat4(1.0f); //Матрица проецирования
    uniform mat4 model      = mat4(1.0f); //Матрица преобразований
    uniform mat4 view       = mat4(1.0f); //Матрица обзора

    //Выходные данные вершинного шейдера
    out VS_OUT{
        vec3 outColor;
        vec2 outUV;
        vec3 outNormal;
        vec3 fragPos;
    }vs_out;

    void main() { 
       //Позиция вершины после применения матрицы преобразований
       vs_out.fragPos = vec3(model * vec4(vPos.x, vPos.y, vPos.z, 1.0f));

       gl_Position =  projection * view * vec4(vs_out.fragPos, 1.0f); 

       //Применение преобразований (перемещения, вращения, масштабирования)
       //к нормалям
       vs_out.outNormal = mat3(transpose(inverse(model))) * vNormal;
       vs_out.outColor = vColor;
       vs_out.outUV    = vUV;
    } 

)glsl"
;

//Исходный код пиксельного шейдера
std::string fShader = R"glsl(             
    #version 440 core                    //Версия GLSL
    #line 42

    uniform vec4 color;

    //Входные данные пиксельного шейдера
    in VS_OUT{
        vec3 outColor;
        vec2 outUV;
        vec3 outNormal;
        vec3 fragPos;
    }fs_in;

    out vec4 fragColor;                  //Выходной цвет пикселя

    uniform sampler2D texture_1;

    void main() { 

       vec3 texture_color = vec3(texture(texture_1, fs_in.outUV));   
  
       //Позиция камеры
       vec3 cameraPos = vec3(2.5, 2.5, 2.5);
       //Направление обзора
       vec3 viewDir   = normalize(cameraPos - fs_in.fragPos);

       //Позиция источника света
       vec3 lightPos  = vec3(-1, -1, 2);
       //Направления луча света для текущего пикселя
       vec3 lightDir  = normalize(lightPos - fs_in.fragPos );
       //Цвет источника света
       vec3 lightColor = vec3(1, 1, 1);        

       //Нормирование значений координат нормали
       //в промежутке [0, 1]
       vec3 nNormal   = normalize(fs_in.outNormal);
       
       vec3 ambient = 0.1f * lightColor; 
       
       //Рассеивание энергии
       vec3 diffuse = max(dot(lightDir, nNormal), 0.0f) * lightColor; 
       
       //Отражение энергии
       vec3 reflectedLightDir = reflect(-lightDir, nNormal); 
       vec3 specular          = 2 * pow(max(dot(reflectedLightDir, viewDir), 0.0f), 32) * lightColor; 

       //Изменение значения выходного цвета пикселя
       fragColor = vec4((ambient + diffuse + specular) * texture_color, 1.0f); 
    } 
)glsl";
#else
//Исходный код вершинного шейдера
std::string vShader = R"glsl(
    #version 440 core                     //Версия GLSL
    #line 16
    layout(location = 0) in vec3 vPos;    //Координаты вершины примитива
    layout(location = 1) in vec3 vColor;  //Цвет вершины примитива
    layout(location = 2) in vec2 vUV;     //UV вершины примитива
    layout(location = 3) in vec3 vNormal; //Нормаль 

    uniform mat4 projection = mat4(1.0f); //Матрица проецирования
    uniform mat4 model      = mat4(1.0f); //Матрица преобразований
    uniform mat4 view       = mat4(1.0f); //Матрица обзора

    //Выходные данные вершинного шейдера
    out VS_OUT{
        vec3 outColor;
        vec2 outUV;
        vec3 outNormal;
        vec3 fragPos;
        vec3 lightValue;
    }vs_out;

    void main() { 
       vs_out.fragPos = vec3(model * vec4(vPos.x, vPos.y, vPos.z, 1.0f));

       gl_Position =  projection * view * vec4(vs_out.fragPos, 1.0f); 

       vs_out.outNormal = mat3(transpose(inverse(model))) * vNormal;
       vs_out.outColor = vColor;
       vs_out.outUV    = vUV;

       vec3 cameraPos = vec3(2.5, 2.5, 2.5);
       vec3 viewDir   = normalize(cameraPos - vs_out.fragPos);

       vec3 lightPos  = vec3(1, 1, 2);
       vec3 lightDir  = normalize(lightPos - vs_out.fragPos );
       vec3 lightColor = vec3(1, 1, 1);        

       vec3 nNormal   = normalize(vs_out.outNormal);
       
       vec3 ambient = 0.1f * lightColor; //
       vec3 diffuse = max(dot(lightDir, nNormal), 0.0f) * lightColor; 
       
       vec3 reflectedLightDir = reflect(-lightDir, nNormal); 
       vec3 specular          = 2 * pow(max(dot(reflectedLightDir, viewDir), 0.0f), 32) * lightColor; 
        //vec3 specular = vec3(0, 0, 0);

        vs_out.lightValue = ambient + diffuse + specular; 
    } 

)glsl"
;

//Исходный код пиксельного шейдера
std::string fShader = R"glsl(             
    #version 440 core                    //Версия GLSL
    #line 42

    uniform vec4 color;

    //Входные данные пиксельного шейдера
    in VS_OUT{
        vec3 outColor;
        vec2 outUV;
        vec3 outNormal;
        vec3 fragPos;
        vec3 lightValue;
    }fs_in;

    out vec4 fragColor;                  //Выходной цвет пикселя

    uniform sampler2D texture_1;

    void main() { 
       vec3 texture_color = vec3(texture(texture_1, fs_in.outUV));   
       fragColor = vec4(fs_in.lightValue * texture_color, 1.0f); //Изменение значения выходного цвета пикселя
    } 
)glsl";
#endif

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

GLFWwindow*         window;
glm::mat4   projection_mat; //Матрица проецирования
glm::mat4         view_mat; //Матрица обзора

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

    //Обработчик события изменения размера окна
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
        #ifdef USE_ORTHO
            float scale = 3.5f;
            projection_mat = glm::ortho(-scale, scale * (float)width / height, -scale * (float)width / height, scale, 0.1f, 1000.f);
        #else
            projection_mat = glm::perspective(glm::radians(90.0f), (float)width / height, 0.1f, 1000.f);
        #endif    
    });
        
    //Загрузка реализаций функций OpenGL из драйверов
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //Загрузить пиксели текстуры в массив img с помощью библиотеки stb_image
    //stbi_set_flip_vertically_on_load(false);
    int width, height, channels;
    unsigned char* img = stbi_load("textures/marble.jpg", &width, &height, &channels, 0);

    if(!img) std::cout << "Изображение не найдено" << std::endl;

    //Создание текстуры
    GLuint texture;
    //Сгенерировать идентификатор
    glGenTextures(1, &texture);
    //Выбрать активную текстуру                                        
    glBindTexture(GL_TEXTURE_2D, texture);
    //Обертка текстуры при UV.x > 1 и UV.x < 0                         
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,      GL_CLAMP_TO_EDGE);  
    //Обертка текстуры при UV.y > 1 и UV.y < 0
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,      GL_CLAMP_TO_EDGE);
    //Вид фильтрации при уменьшении поверхности отображения 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
    //Вид фильтрации при увеличении поверхности отображения
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    //Считывать по одному байту из строки пикселей
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                             

    //Используется при GL_CLAMP_TO_BORDER
    float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //Формат либо RGB или RGBA
    switch (channels) {
    case 3:
        //Загрузить пиксели текстуры в объект texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        break;
    case 4:
        //Загрузить пиксели текстуры в объект texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        break;
    default:
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);

    //Массив вершин
    auto vertices = CreatePlaneArray();//Плоскость
    //auto vertices = CreatePyramidArray();//Пирамида
    //auto vertices = CreateCubeArray();//Куб

    GLuint vao;                         //Массив вершин (Vertex Array)
    glGenVertexArrays(1, &vao);         //Сгенерировать идентификатор
    glBindVertexArray(vao);             //Выбрать массив вершин 

    GLuint vbo;                         //Буфер вершин (Vertex Buffer)
    glGenBuffers(1, &vbo);              //Сгенерировать идентификатор
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //Выбрать буфер

    //Заполнить буфер данными
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    
    GLsizei stride = 11;
    //Определить интерпретацию считывания из буфера для идентификатора 0 в шейдере
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GL_FLOAT), (void*)0);
    //Определить интерпретацию считывания из буфера для идентификатора 1 в шейдере
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GL_FLOAT), (void*)(sizeof(float) * 3));
    //Определить интерпретацию считывания из буфера для идентификатора 2 в шейдере
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GL_FLOAT), (void*)(sizeof(float) * 6));
    //Определить интерпретацию считывания из буфера для идентификатора 3 в шейдере
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GL_FLOAT), (void*)(sizeof(float) * 8));

    //Разрешить передачу данных по нулевому идентификатору
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    GLuint program = CreateProgram();

    #ifdef USE_ORTHO
        float scale = 3.5f;
        projection_mat = glm::ortho(-scale, scale * 800.f/600.f, -scale * 800.f/600.f, scale, 0.1f, 1000.f);
    #else 
        projection_mat = glm::perspective(glm::radians(90.0f), 800.f/600.f, 0.1f, 1000.f);
    #endif

    glm::vec3 cameraPos = {2.5f, 2.5f, 2.5f};
    view_mat = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    //Время итерации главного цикла
    float deltaTime = 0.0f;
    
    //Угол поворота
    float angle = 0.0f;
    float speed = 50.f;

    //Тест глубины
    glEnable(GL_DEPTH_TEST);

    //Пока окно не закрыто
    while (!glfwWindowShouldClose(window))
    {
        //Время перед выполнением итерации
        auto begin = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       //Очистить экран
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);  //Цвет очистки экрана

        glUseProgram(program);//Выбираем активную программу

        //Передача матрицы проецирования в шейдер
        glUniformMatrix4fv(
            glGetUniformLocation(program, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection_mat)
        );

        //Передача матрицы обзора в шейдер
        glUniformMatrix4fv(
            glGetUniformLocation(program, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view_mat)
        );

        //Вращение объекта
        {
            angle += deltaTime * speed;
            SetTransform(program, { 0.5, 0.5, 0}, { 0, 0, angle }, {3, 3, 2});
        }

        
        glBindTexture(GL_TEXTURE_2D, texture);                //Выбрать активную текстуру
        glBindVertexArray(vao);                               //Выбираем активный массив вершин
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 11);   //Отрисовать vertices.size/11 вершин
        

        if(glfwGetKey(window, GLFW_KEY_W))
            std::cout << "Клавиша нажата" << std::endl;

        //Чередовать буферы с изображением
        glfwSwapBuffers(window);
        //Обработать события ввода
        glfwPollEvents();

        //Время после выполнения итерации
        auto end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f;
    }


    //Освободить ресурсы
    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);

    return 0;
}