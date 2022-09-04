#include <glad/glad.h>
#include <GLFW/glfw3.h> //���������� GLFW
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//�������� ��� ���������� �������
std::string vShader = R"glsl(
    #version 440 core                     //������ GLSL
    #line 16
    layout(location = 0) in vec2 vPos;    //���������� ������� ���������
    layout(location = 1) in vec3 vColor;  //���� ������� ���������
    layout(location = 2) in vec2 vUV;  //���� ������� ���������

    uniform mat4 projection = mat4(1.0f);
    uniform mat4 model      = mat4(1.0f); //������� ��������������

    //�������� ������ ���������� �������
    out VS_OUT{
        vec3 outColor;
        vec2 outUV;
    }vs_out;

    void main() { 
       vs_out.outColor = vColor;
       vs_out.outUV    = vUV;
       gl_Position =  projection * model * vec4(vPos.x, vPos.y, 0.0f, 1.0f); 
    } 

)glsl"
;

//�������� ��� ����������� �������
std::string fShader = R"glsl(             
    #version 440 core                    //������ GLSL
    #line 42

    uniform vec4 color;

    //������� ������ ����������� �������
    in VS_OUT{
        vec3 outColor;
        vec2 outUV;
    }fs_in;

    out vec4 fragColor;                  //�������� ���� �������

    uniform sampler2D texture_1;

    void main() { 
       vec3 texture_color = vec3(texture(texture_1, fs_in.outUV));     
       fragColor = vec4(texture_color.r, texture_color.g, texture_color.b, 1.0f); //��������� �������� ��������� ����� �������
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
glm::mat4 projection_mat;


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

    //���������� ������� ��������� ������� ����
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
        projection_mat = glm::ortho(0.f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
        
    });
        
    //�������� ���������� ������� OpenGL �� ���������
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //��������� ������� �������� � ������ img � ������� ���������� stb_image
    //stbi_set_flip_vertically_on_load(false);
    int width, height, channels;
    unsigned char* img = stbi_load("textures/sand.jpg", &width, &height, &channels, 0);

    if(!img)
        std::cout << "����������� �� �������" << std::endl;

    //�������� ��������
    GLuint texture;
    //������������� �������������
    glGenTextures(1, &texture);
    //������� �������� ��������                                        
    glBindTexture(GL_TEXTURE_2D, texture);
    //������� �������� ��� UV.x > 1 � UV.x < 0                         
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,      GL_CLAMP_TO_EDGE);  
    //������� �������� ��� UV.y > 1 � UV.y < 0
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,      GL_CLAMP_TO_EDGE);
    //��� ���������� ��� ���������� ����������� ����������� 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
    //��� ���������� ��� ���������� ����������� �����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    //��������� �� ������ ����� �� ������ ��������
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);                             

    //������������ ��� GL_CLAMP_TO_BORDER
    float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //������ ���� RGB ��� RGBA
    switch (channels) {
    case 3:
        //��������� ������� �������� � ������ texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        break;
    case 4:
        //��������� ������� �������� � ������ texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        break;
    default:
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);


    float uv_max =  1.0f;
    float uv_min =  0.0f;

    //������ ������
    const float vertices[] = {
      //����������     //����   //UV
          0.5f,  0.5f, 1, 0, 0, uv_max, uv_max, //------------
          0.5f, -0.5f, 0, 1, 0, uv_max, uv_min, //���������� 1
         -0.5f, -0.5f, 0, 0, 1, uv_min, uv_min, //------------
         -0.5f, -0.5f, 1, 0, 0, uv_min, uv_min, //------------
         -0.5f,  0.5f, 0, 1, 0, uv_min, uv_max, //����������� 2
          0.5f,  0.5f, 0, 0, 1, uv_max, uv_max, //------------
    };  


    ////������ ������
    //const float vertices[] = {
    //    //����������     //����   //UV
    //        0.5f,  0.5f, 1, 0, 0, 1, 1, //------------
    //        0.5f, -0.5f, 0, 1, 0, 1, 0, //���������� 1
    //       -0.5f, -0.5f, 0, 0, 1, 0, 0, //------------
    //       -0.5f, -0.5f, 1, 0, 0, 0, 0, //------------
    //       -0.5f,  0.5f, 0, 1, 0, 0, 1, //����������� 2
    //        0.5f,  0.5f, 0, 0, 1, 1, 1, //------------
    //};



    GLuint vao;                         //������ ������ (Vertex Array)
    glGenVertexArrays(1, &vao);         //������������� �������������
    glBindVertexArray(vao);             //������� ������ ������ 

    GLuint vbo;                         //����� ������ (Vertex Buffer)
    glGenBuffers(1, &vbo);              //������������� �������������
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //������� �����
    //��������� ����� �������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    //���������� ������������� ���������� �� ������ ��� �������������� 0 � �������
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (void*)0);

    //���������� ������������� ���������� �� ������ ��� �������������� 1 � �������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (void*)(sizeof(float) * 2));

    //���������� ������������� ���������� �� ������ ��� �������������� 2 � �������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (void*)(sizeof(float) * 5));

    //��������� �������� ������ �� �������� ��������������
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLuint program = CreateProgram();

    //������� ��������
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(400, 300, 0));
    //������� ���������������
    glm::mat4 scaleMat       = glm::scale(glm::mat4(1.0f),     glm::vec3(300, 300, 0));
    //������� ��������  
    glm::mat4 rotateMat      = glm::rotate(glm::radians(0.f),  glm::vec3(0,    0,  1));
    //������� ��������������
    glm::mat4 modelMat = translationMat * rotateMat * scaleMat;

    projection_mat = glm::ortho(0.f, 800.f, 600.f, 0.0f, -1.0f, 1.0f);


    //����� �������� �������� �����
    float deltaTime = 0.0f;

    //���� ���� �� �������
    while (!glfwWindowShouldClose(window))
    {
        //����� ����� ����������� ��������
        auto begin = std::chrono::high_resolution_clock::now();

        glClear(GL_COLOR_BUFFER_BIT);       //�������� �����
        glClearColor(0.3, 0.3, 0.3, 1.0f);  //���� ������� ������

        //�������� ������� �������� � ������
        glUseProgram(program);//�������� �������� ���������
        glUniformMatrix4fv(
            glGetUniformLocation(program, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection_mat)
        );

        //�������� ������� �������������� � ������
        glUseProgram(program);
        glUniformMatrix4fv(
            glGetUniformLocation(program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelMat)
        );
            
        glBindTexture(GL_TEXTURE_2D, texture);//������� �������� ��������
        glBindVertexArray(vao);               //�������� �������� ������ ������
        glDrawArrays(GL_TRIANGLES, 0, 6);     //���������� ����� ������


        if(glfwGetKey(window, GLFW_KEY_W))
            std::cout << "������� ������" << std::endl;

        //���������� ������ � ������������
        glfwSwapBuffers(window);
        //���������� ������� �����
        glfwPollEvents();

        //����� ����� ���������� ��������
        auto end = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f;
    }





    //���������� �������
    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);

    return 0;
}