#pragma once

#include "MazeGame.h"
#include <thread>

void showStartScreen(GLFWwindow* window) {
    //��ȡ���ڿ��
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    //���ñ�����ɫΪ��ɫ
    glClearColor(1.0, 1.0, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //���ơ���ʼ��Ϸ����ť���������һ�����α�ʾ���ɸ�����Ҫ������
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(0.2f, 0.1f);
    glVertex2f(-0.2f, 0.1f);
    glEnd();

    //���ư�ť����
    std::string startText = "Start";
    glColor3f(0.0f, 0.0f, 0.0f);

    //�����ı����
    double textWidth = 0.0;
    for (char c : startText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //���㰴ť��������
    double buttonCenterX = (windowWidth / 2.0) / 400.0 - 1.0;
    double buttonCenterY = 0.0f;

    //�����ı���ʼ������ʵ�����¾���
    double textStartX = buttonCenterX - (textWidth / 2.0) / 400.0;
    const double FONT_HEIGHT = 24.0f;
    double textStartY = buttonCenterY - (FONT_HEIGHT / 2.0f) / 400.0f;

    //�����ı�λ�ò�����
    glRasterPos2f(textStartX, textStartY);
    for (char c : startText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glfwSwapBuffers(window);

    //�ȴ��������ʼ��Ϸ
    while (true) {
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            // ת�����굽 -1��1��Χ
            x = (x / 400.0) - 1.0;
            y = -(y / 400.0) + 1.0;
            // ����Ƿ����˰�ť����
            if (x >= -0.5f && x <= 0.5f && y >= -0.2f && y <= 0.2f) {
                break;
            }
        }
    }
}


void showEndScreen(GLFWwindow* window, bool isWon) {
    //��ȡ���ڿ�Ⱥ͸߶�
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    //���ñ�����ɫ
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::string endText;
    if (isWon) {
        endText = "congratulations��";
    }
    else {
        endText = "Run out of time...";
    }

    glColor3f(0.0f, 0.0f, 0.0f);

    //�����ı����
    double textWidth = 0.0;
    for (char c : endText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //�����ı��߶�
    double textHeight = 24;

    //�����ı��ڴ����еľ���λ��
    double textX = (windowWidth / 2.0 - textWidth / 2.0) / 400.0 - 1.0;
    double textY = (windowHeight / 2.0 - textHeight / 2.0) / 400.0 - 1.0;

    //�����ı�λ�ò�����
    glRasterPos2f(textX, textY);
    for (char c : endText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glfwSwapBuffers(window);

    //�ȴ�3���رմ���
    std::this_thread::sleep_for(std::chrono::seconds(3));
    glfwSetWindowShouldClose(window, GL_TRUE);
}