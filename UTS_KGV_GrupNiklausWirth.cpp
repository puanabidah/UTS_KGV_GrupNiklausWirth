// UTS_KGV_GrupNiklausWirth.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Util.h"

// Warna default
float color1[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
float color2[] = { 1.0f, 1.0f, 1.0f, 1.0f };  
float color3[] = { 0.4f, 0.2f, 0.1f, 1.0f };
float color4[] = { 1.0f, 0.2f, 0.9f, 1.0f };
float color5[] = { 0.0f, 0.8f, 0.2f, 1.0f };
float color6[] = { 1.0f, 0.4f, 0.5f, 1.0f };
float color7[] = { 0.0f, 1.0f, 0.0f, 1.0f };
float color8[] = { 1.0f, 1.0f, 0.0f, 1.0f }; 

unsigned int program;
GLint color1Loc, color2Loc, color3Loc, color4Loc, color5Loc, color6Loc, color7Loc, color8Loc, dot1Loc, dot2Loc, scaleLoc;
float xpusat, ypusat, r = 1, phi = 3.14, n_sisi = 8, x, y, z = 0.0f, s = 0.5f, inc = 0.05f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        for (int i = 0; i < 4; i++)
        {
            color1[i] = (rand() * 1.0f) / RAND_MAX;
            color2[i] = (rand() * 1.0f) / RAND_MAX;
            color3[i] = (rand() * 1.0f) / RAND_MAX;
            color4[i] = (rand() * 1.0f) / RAND_MAX;
            color5[i] = (rand() * 1.0f) / RAND_MAX;
            color6[i] = (rand() * 1.0f) / RAND_MAX;
            color7[i] = (rand() * 1.0f) / RAND_MAX;
            color8[i] = (rand() * 1.0f) / RAND_MAX;
        }

        glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);
        glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);
        glUniform4f(color3Loc, color3[0], color3[1], color3[2], color3[3]);
        glUniform4f(color4Loc, color4[0], color4[1], color4[2], color4[3]);
        glUniform4f(color5Loc, color5[0], color5[1], color5[2], color5[3]);
        glUniform4f(color6Loc, color6[0], color6[1], color6[2], color6[3]);
        glUniform4f(color7Loc, color7[0], color7[1], color7[2], color7[3]);
        glUniform4f(color8Loc, color8[0], color8[1], color8[2], color8[3]);
    }

    // Memperbesar skala
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        s = s + 0.1f;
        glUniform1f(scaleLoc, s);
    }

    // Memperkecil skala
    else if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        s = s - 0.1f;
        glUniform1f(scaleLoc, s);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Projek UTS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);

    /* Initialize GLEW */
    GLenum err = glewInit();

    // isi dari vertexnya masih kosong, karena akan dihitung dgn rumus
    float vertices[18] = { };

    // Menghitung nilai x dan y menggunakan rumus berikut
    int index = 0; 
    for (int i = 0; i < n_sisi; i++)
    {
        x = xpusat + r * cos(2 * phi * i / n_sisi);
        y = ypusat + r * sin(2 * phi * i / n_sisi);
        vertices[index] = x;
        index = index + 1;
        vertices[index] = y;
        index = index + 1;
    }
    vertices[16] = xpusat;
    vertices[17] = ypusat;

    // Menyimpan data ke index buffer
    unsigned int indexArr[] = {
        0, 1, 8,
        1, 2, 8,
        2, 3, 8,
        3, 4, 8,
        4, 5, 8,
        5, 6, 8,
        6, 7, 8,
        7, 0, 8
    };

    /* untuk pengiriman data ke GPU menggunakan vertex buffer */
    unsigned int vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertices, GL_STATIC_DRAW);

    /* untuk pengiriman data ke GPU menggunakan index buffer */
    unsigned int indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 24, indexArr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* untuk membaca file vertex dan file fragment */
    string vertexString = readFile("vertex.vert");
    string fragmentString = readFile("fragment.frag");

    /* konversi vertex dan fragment string tadi ke dalam char */
    const char* vertexChar = vertexString.c_str();
    const char* fragmentChar = fragmentString.c_str();

    /* create shader */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* intruksi source shader */
    glShaderSource(vertexShader, 1, &vertexChar, NULL);
    glShaderSource(fragmentShader, 1, &fragmentChar, NULL);

    /* compile shader */
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glUseProgram(program);
    scaleLoc = glGetUniformLocation(program, "scale");
    glUniform1f(scaleLoc, 0.5f);

    //membuat titik 1
    dot1Loc = glGetUniformLocation(program, "titik1");
    glUniform1f(dot1Loc, indexArr[0]);

    //membuat titik 2
    dot2Loc = glGetUniformLocation(program, "titik2");
    glUniform1f(dot2Loc, indexArr[1]);

    color1Loc = glGetUniformLocation(program, "color1");
    glUniform4f(color1Loc, color1[0], color1[1], color1[2], color1[3]);

    color2Loc = glGetUniformLocation(program, "color2");
    glUniform4f(color2Loc, color2[0], color2[1], color2[2], color2[3]);

    color3Loc = glGetUniformLocation(program, "color3");
    glUniform4f(color3Loc, color3[0], color3[1], color3[2], color3[3]);

    color4Loc = glGetUniformLocation(program, "color4");
    glUniform4f(color4Loc, color4[0], color4[1], color4[2], color4[3]);

    color5Loc = glGetUniformLocation(program, "color5");
    glUniform4f(color5Loc, color5[0], color5[1], color5[2], color5[3]);

    color6Loc = glGetUniformLocation(program, "color6");
    glUniform4f(color6Loc, color6[0], color6[1], color6[2], color6[3]);

    color7Loc = glGetUniformLocation(program, "color7");
    glUniform4f(color7Loc, color7[0], color7[1], color7[2], color7[3]);

    color8Loc = glGetUniformLocation(program, "color8");
    glUniform4f(color8Loc, color8[0], color8[1], color8[2], color8[3]);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(color1Loc, z, color1[1], color1[2], color1[3]);
        glUniform4f(color2Loc, z, color2[1], color2[2], color2[3]);
        glUniform4f(color3Loc, z, color3[1], color3[2], color3[3]);
        glUniform4f(color4Loc, z, color4[1], color4[2], color4[3]);
        glUniform4f(color5Loc, z, color5[1], color5[2], color5[3]);
        glUniform4f(color6Loc, z, color6[1], color6[2], color6[3]);
        glUniform4f(color7Loc, z, color7[1], color7[2], color7[3]);
        glUniform4f(color8Loc, z, color8[1], color8[2], color8[3]);

        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
        
        // untuk perhitungan animasi perubahan warna
        if (z > 1.0f) inc = -0.005f; 
        else if (z < 0.0f) inc = 0.005f;
        z = z + inc;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}