#include <glad/glad.h>
#include <GLFW/glfw3.h> //���������� GLFW
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <chrono>

//�������� ��� ���������� �������
std::string vShader = R"glsl(
    #version 440 core                     //������ GLSL
    #line 14
    layout(location = 0) in vec2 vPos;    //���������� ������� ���������
    layout(location = 1) in vec3 vColor;  //���� ������� ���������

    uniform mat4 model = mat4(1.0f); //������� ��������������

    //�������� ������ ���������� �������
    out VS_OUT{
        vec3 outColor;
    }vs_out;

    void main() { 
       vs_out.outColor = vColor;
       gl_Position =  model * vec4(vPos.x, vPos.y, 0.0f, 1.0f); 
    } 

)glsl"
;

//�������� ��� ����������� �������
std::string fShader = R"glsl(             
    #version 440 core                    //������ GLSL
    #line 36

    uniform vec4 color;

    //������� ������ ����������� �������
    in VS_OUT{
        vec3 outColor;
    }fs_in;

    out vec4 fragColor;                  //�������� ���� �������

    void main() { 
       fragColor = vec4(fs_in.outColor.r, fs_in.outColor.g, fs_in.outColor.b, 1.0f); //��������� �������� ��������� ����� �������
    } 
)glsl"
;

//�������� ����������
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

    //������������ �������� ��� �� std::string � const char *
    const char* vertex_shader_src   = vShader.c_str();
    const char* fragment_shader_src = fShader.c_str();
    
    glShaderSource(vertex_shader, 1, &vertex_shader_src, 0);
    glCompileShader(vertex_shader);
    Check(vertex_shader); //�������� ����������

    glShaderSource(pixel_shader, 1, &fragment_shader_src, 0);
    glCompileShader(pixel_shader);
    Check(pixel_shader); //�������� ����������

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, pixel_shader);
    glLinkProgram(program);   
    Check(program, true); //�������� ����������

    glDeleteShader(vertex_shader);
    glDeleteShader(pixel_shader);

    return program;
}



GLFWwindow* window;

int main() {
    setlocale(LC_ALL, "rus");

    glfwInit();//������������� GLFW

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                //������� ������ GL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                //������� ������ GL
                         
                            //���������: ������, ������, �������� ����
    window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr); //�������� ����
    
    //��������� �������� OpenGL ������� ��� ���������� ����
    glfwMakeContextCurrent(window);
 
    //������������ �������������
    glfwSwapInterval(1);

    //�������� ���������� ������� OpenGL �� ���������
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //������ ������
    const float vertices[] = {
      //����������     //����
          0.5f,  0.5f, 1, 0, 0, //------------
          0.5f, -0.5f, 0, 1, 0, //���������� 1
         -0.5f, -0.5f, 0, 0, 1, //------------
         -0.5f, -0.5f, 1, 0, 0, //------------
         -0.5f,  0.5f, 0, 1, 0, //����������� 2
          0.5f,  0.5f, 0, 0, 1, //------------
    };  


    GLuint vao;                         //������ ������ (Vertex Array Object)
    glGenVertexArrays(1, &vao);         //������������� �������������
    glBindVertexArray(vao);             //������� ������ ������ 

    GLuint vbo;                         //����� ������ (Vertex Buffer Object)
    glGenBuffers(1, &vbo);              //������������� �������������
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //������� �����
    //��������� ����� �������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    //���������� ������������� ���������� �� ������ ��� �������������� 0 � �������
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

    //���������� ������������� ���������� �� ������ ��� �������������� 1 � �������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(float) * 2));

    //��������� �������� ������ �� �������� ��������������
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLuint program = CreateProgram();

    //������� ��������
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0));
    //������� ���������������
    glm::mat4 scaleMat       = glm::scale(glm::mat4(1.0f),     glm::vec3(0.5, 0.5, 0));
    //������� ��������  
    glm::mat4 rotateMat      = glm::rotate(glm::radians(45.f), glm::vec3(0,     0, 1));
    //������� ��������������
    glm::mat4 modelMat = translationMat * rotateMat * scaleMat;

    //����� �������� �������� �����
    float deltaTime = 0.0f;
    //���� ���� �� �������
    while (!glfwWindowShouldClose(window))
    {
        //����� ����� ����������� ��������
        auto begin = std::chrono::high_resolution_clock::now();


        glClear(GL_COLOR_BUFFER_BIT);       //�������� �����
        glClearColor(0.3, 0.3, 0.3, 1.0f);  //���� ������� ������

        //�������� ������� �������������� � ������
        glUseProgram(program); //�������� �������� ���������
        glUniformMatrix4fv(
            glGetUniformLocation(program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelMat)
        );

        glBindVertexArray(vao);             //�������� �������� ������ ������
        glDrawArrays(GL_TRIANGLES, 0, 6);   //���������� ����� ������


        if(glfwGetKey(window, GLFW_KEY_W))
            std::cout << "������� ������" << std::endl;

        //���������� ������� �����
        glfwPollEvents();
        //���������� ������ � ������������
        glfwSwapBuffers(window);


        //����� ����� ���������� ��������
        auto end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f;
    }

    //���������� �������
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}