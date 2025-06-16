// Autor: Nedeljko Tesanovic
// Opis: Testiranje dubine, Uklanjanje lica, Transformacije, Prostori i Projekcije

#define _CRT_SECURE_NO_WARNINGS
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

//GLM biblioteke
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
static unsigned loadImageToTexture(const char* filePath); 
unsigned int createShader(const char* vsSource, const char* fsSource);

float yCube1 = 0.0f;
float yCube2 = 0.0f;
float yPyramid = 0.0f;

bool moveCube1 = true;
bool moveCube2 = true;
bool movePyramid = true;

bool key1PressedLastFrame = false;
bool key2PressedLastFrame = false;
bool key3PressedLastFrame = false;

int FPS = 60;
double FRAME_TIME = 1.0 / FPS;

int main(void)
{

   
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24); 

    GLFWwindow* window;
    unsigned int wWidth = 1000;
    unsigned int wHeight = 1000;
    const char wTitle[] = "3D Projekat SV63/2021";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    
    glfwMakeContextCurrent(window);

    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int textureShader = createShader("basictexture.vert", "basictexture.frag");

    float cubeVertices[] = {
        //  X      Y      Z       R     G     B     A
   // Prednje lice (z = 0.25)
   -0.25f, -0.25f,  0.25f,   1, 0, 0, 1,
    0.25f, -0.25f,  0.25f,   1, 0, 0, 1,
    0.25f,  0.25f,  0.25f,   1, 0, 0, 1,
    0.25f,  0.25f,  0.25f,   1, 0, 0, 1,
   -0.25f,  0.25f,  0.25f,   1, 0, 0, 1,
   -0.25f, -0.25f,  0.25f,   1, 0, 0, 1,

   // Zadnje lice (z = -0.25)
   -0.25f, -0.25f, -0.25f,   0, 1, 0, 1,
   -0.25f,  0.25f, -0.25f,   0, 1, 0, 1,
    0.25f,  0.25f, -0.25f,   0, 1, 0, 1,
    0.25f,  0.25f, -0.25f,   0, 1, 0, 1,
    0.25f, -0.25f, -0.25f,   0, 1, 0, 1,
   -0.25f, -0.25f, -0.25f,   0, 1, 0, 1,

   // Levo
   -0.25f,  0.25f,  0.25f,   0, 0, 1, 1,
   -0.25f,  0.25f, -0.25f,   0, 0, 1, 1,
   -0.25f, -0.25f, -0.25f,   0, 0, 1, 1,
   -0.25f, -0.25f, -0.25f,   0, 0, 1, 1,
   -0.25f, -0.25f,  0.25f,   0, 0, 1, 1,
   -0.25f,  0.25f,  0.25f,   0, 0, 1, 1,

   // Desno
    0.25f,  0.25f,  0.25f,   1, 1, 0, 1,
    0.25f, -0.25f,  0.25f,   1, 1, 0, 1,
    0.25f, -0.25f, -0.25f,   1, 1, 0, 1,
    0.25f, -0.25f, -0.25f,   1, 1, 0, 1,
    0.25f,  0.25f, -0.25f,   1, 1, 0, 1,
    0.25f,  0.25f,  0.25f,   1, 1, 0, 1,

    // Gore
    -0.25f,  0.25f, -0.25f,   1, 0, 1, 1,
    -0.25f,  0.25f,  0.25f,   1, 0, 1, 1,
     0.25f,  0.25f,  0.25f,   1, 0, 1, 1,
     0.25f,  0.25f,  0.25f,   1, 0, 1, 1,
     0.25f,  0.25f, -0.25f,   1, 0, 1, 1,
    -0.25f,  0.25f, -0.25f,   1, 0, 1, 1,

    // Dole
    -0.25f, -0.25f, -0.25f,   0, 1, 1, 1,
     0.25f, -0.25f, -0.25f,   0, 1, 1, 1,
     0.25f, -0.25f,  0.25f,   0, 1, 1, 1,
     0.25f, -0.25f,  0.25f,   0, 1, 1, 1,
    -0.25f, -0.25f,  0.25f,   0, 1, 1, 1,
    -0.25f, -0.25f, -0.25f,   0, 1, 1, 1

    };

 
    unsigned int stride = (3 + 4) * sizeof(float); 
    
    unsigned int VAO[4];
  

    unsigned int VBO[4];
 

    //-------------------------------------- KOCKA KUTIJA ------------------------------
    glGenVertexArrays(1, &VAO[1]);
    glBindVertexArray(VAO[1]);


    glGenBuffers(1, &VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float pyramidVertices[] = {
        //BAZA
        -0.25f, 0.0f, -0.25f,  1, 0, 0, 1,
         0.25f, 0.0f,  0.25f,  1, 0, 0, 1,
         0.25f, 0.0f, -0.25f,  1, 0, 0, 1,

         0.25f, 0.0f,  0.25f,  1, 0, 0, 1,
        -0.25f, 0.0f, -0.25f,  1, 0, 0, 1,
        -0.25f, 0.0f,  0.25f,  1, 0, 0, 1,

        // Bocne strane piramide 
        -0.25f, 0.0f, -0.25f,  0, 1, 0, 1,
         0.0f,  0.5f,  0.0f,   0, 1, 0, 1,
         0.25f, 0.0f, -0.25f,  0, 1, 0, 1,

         0.25f, 0.0f, -0.25f,  0, 0, 1, 1,
         0.0f,  0.5f,  0.0f,   0, 0, 1, 1,
         0.25f, 0.0f,  0.25f,  0, 0, 1, 1,

         0.25f, 0.0f,  0.25f,  1, 1, 0, 1,
         0.0f,  0.5f,  0.0f,   1, 1, 0, 1,
        -0.25f, 0.0f,  0.25f,  1, 1, 0, 1,

        -0.25f, 0.0f,  0.25f,  1, 0, 1, 1,
         0.0f,  0.5f,  0.0f,   1, 0, 1, 1,
        -0.25f, 0.0f, -0.25f,  1, 0, 1, 1
    };


    glGenVertexArrays(1, &VAO[0]);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1, &VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

 
    float platformVertices[] = {
        //        X       Y       Z       R     G     B     A
        -1.0f, -0.26f, -1.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // 1
         1.0f, -0.26f,  1.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // 3
         1.0f, -0.26f, -1.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // 2

         1.0f, -0.26f,  1.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // 3
        -1.0f, -0.26f, -1.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // 1
        -1.0f, -0.26f,  1.0f,   0.0f, 0.0f, 0.0f, 1.0f   // 4
    };

    glGenVertexArrays(1, &VAO[2]);
    glBindVertexArray(VAO[2]);

    glGenBuffers(1, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(platformVertices), platformVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int textureStride = (2 + 2) * sizeof(float);
    float nameTextureVertices[] = {
        //   X      Y      S    T
         0.9f,  0.9f,  1.0f, 1.0f,  // top right
         0.5f,  0.9f,  0.0f, 1.0f,  // top left
         0.9f,  0.7f,  1.0f, 0.0f,  // bottom right
         0.5f,  0.7f,  0.0f, 0.0f   // bottom left
    };


    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameTextureVertices), nameTextureVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned nameTexture = loadImageToTexture("res/potpis.png"); //Ucitavamo teksturu
    glBindTexture(GL_TEXTURE_2D, nameTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(textureShader);
    unsigned uTexLoc1 = glGetUniformLocation(textureShader, "uTex");
    glUniform1i(uTexLoc1, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    glUseProgram(0);
    

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    
   
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");

    glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 5.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f); 
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
   
    float cameraSpeed = 0.0005f;
    float yaw = -90.0f; 
 
    
    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
    


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++
    glUseProgram(unifiedShader); //Slanje default vrijednosti uniformi
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //(Adresa matrice, broj matrica koje saljemo, da li treba da se transponuju, pokazivac do matrica)
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
    glBindVertexArray(VAO[1]);

   
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glFrontFace(GL_CCW); 
    glCullFace(GL_BACK); 
    glEnable(GL_CULL_FACE);
    
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        

        auto startTime = std::chrono::high_resolution_clock::now();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float cameraSpeed = 0.5f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        // Rotacija kamere (Q/E)
        float angleSpeed = 0.5f;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            float angle = glm::radians(angleSpeed);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
            cameraFront = glm::vec3(rotation * glm::vec4(cameraFront, 0.0f));
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            float angle = glm::radians(-angleSpeed);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
            cameraFront = glm::vec3(rotation * glm::vec4(cameraFront, 0.0f));
        }
 
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + glm::normalize(cameraFront), cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
       
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        float time = glfwGetTime();
        float baseY = 0.5f;  

        if (moveCube1)
            yCube1 = baseY + sin(time) * 0.2f;  

        if (moveCube2)
            yCube2 = baseY + cos(time) * 0.2f;

        if (movePyramid)
            yPyramid = baseY + sin(time * 1.5f) * 0.2f;
      
        // 1
        bool key1CurrentlyPressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
        if (key1CurrentlyPressed && !key1PressedLastFrame)
            moveCube1 = !moveCube1;
        key1PressedLastFrame = key1CurrentlyPressed;

        // 2
        bool key2CurrentlyPressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
        if (key2CurrentlyPressed && !key2PressedLastFrame)
            moveCube2 = !moveCube2;
        key2PressedLastFrame = key2CurrentlyPressed;

        // 3
        bool key3CurrentlyPressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
        if (key3CurrentlyPressed && !key3PressedLastFrame)
            movePyramid = !movePyramid;
        key3PressedLastFrame = key3CurrentlyPressed;


        ////-------------------------------------------------- POTPIS -------------------------------------
        glDisable(GL_DEPTH_TEST);
        glUseProgram(textureShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glBindVertexArray(VAO[3]);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(unifiedShader);


        // ------------------- PLATFORMA -------------------

        glm::mat4 modelPlatform = glm::mat4(1.0f);
        modelPlatform = glm::translate(modelPlatform, glm::vec3(0.0f, -0.26f, 0.0f));
        modelPlatform = glm::scale(modelPlatform, glm::vec3(4.0f, 1.0f, 4.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlatform));
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        //------------- KOCKE -------------------------------

       
        glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, yCube1, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, yCube2, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // ----------------- PIRAMIDA ------------------------------
        float scale = 1.0f + 0.5f * sin(time * 2.0f);  

        glm::mat4 modelPyramid = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, yPyramid, 0.0f));
        modelPyramid = glm::scale(modelPyramid, glm::vec3(scale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPyramid));
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        double sleepTime = FRAME_TIME - elapsed.count();
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    glDeleteBuffers(4, VBO);
    glDeleteVertexArrays(4, VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
