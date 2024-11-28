#pragma once

#include "MazeGame.h"
#include <thread>

#include "stb_image.h"

GLuint startTexture, winTexture, loseTexture;
void showStartScreen(GLFWwindow* window) {
    //��ȡ���ڿ��
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    //���ر���
    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/start.png", &width, &height, &nrChannels, 0);
    if (data) {
        //std::cout << width << "," << height << "," << nrChannels << std::endl;
        glGenTextures(1, &startTexture);
        glBindTexture(GL_TEXTURE_2D, startTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        std::cout << "�޷����ؿ�ʼ����: " << stbi_failure_reason() << std::endl;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, startTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0, 1.0); 
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0, -1.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0, -1.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);

    //�ȴ��������ʼ��Ϸ
    while (true) {
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            //ת�����굽 -1��1��Χ
            x = (x / maze_pixel) - 1.0;
            y = -(y / maze_pixel) + 1.0;
            //����Ƿ����˰�ť����
            if (x >= -0.5f && x <= 0.5f && y >= -0.2f && y <= 0.2f) {
                break;
            }
        }
    }
}


void showEndScreen(GLFWwindow* window, bool isWon) {

    glColor3f(1.0f, 1.0f, 1.0f);

    //���ر���
    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/win.png", &width, &height, &nrChannels, 0);
    if (data) {
        //std::cout << width << "," << height << "," << nrChannels << std::endl;
        glGenTextures(1, &winTexture);
        glBindTexture(GL_TEXTURE_2D, winTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        std::cout << "�޷�����win����: " << stbi_failure_reason() << std::endl;
    }

    data = stbi_load("resources/lose.png", &width, &height, &nrChannels, 0);
    if (data) {
        //std::cout << width << "," << height << "," << nrChannels << std::endl;
        glGenTextures(1, &loseTexture);
        glBindTexture(GL_TEXTURE_2D, loseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        std::cout << "�޷�����lose����: " << stbi_failure_reason() << std::endl;
    }


    //��ȡ���ڿ�Ⱥ͸߶�
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    std::string endText;
    if (isWon) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, winTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0, -1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0, -1.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, loseTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0, 1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0, -1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0, -1.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    glfwSwapBuffers(window);

    //�ȴ�3���رմ���
    std::this_thread::sleep_for(std::chrono::seconds(3));
    glfwSetWindowShouldClose(window, GL_TRUE);
}