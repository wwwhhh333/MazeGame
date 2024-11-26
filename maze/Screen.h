#pragma once

#include "MazeGame.h"
#include <thread>

void showStartScreen(GLFWwindow* window) {
    //获取窗口宽度
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    //设置背景颜色为蓝色
    glClearColor(1.0, 1.0, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //绘制“开始游戏”按钮（这里简单用一个矩形表示，可根据需要美化）
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(0.2f, 0.1f);
    glVertex2f(-0.2f, 0.1f);
    glEnd();

    //绘制按钮文字
    std::string startText = "Start";
    glColor3f(0.0f, 0.0f, 0.0f);

    //计算文本宽度
    double textWidth = 0.0;
    for (char c : startText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //计算按钮中心坐标
    double buttonCenterX = (windowWidth / 2.0) / 400.0 - 1.0;
    double buttonCenterY = 0.0f;

    //计算文本起始坐标以实现上下居中
    double textStartX = buttonCenterX - (textWidth / 2.0) / 400.0;
    const double FONT_HEIGHT = 24.0f;
    double textStartY = buttonCenterY - (FONT_HEIGHT / 2.0f) / 400.0f;

    //设置文本位置并绘制
    glRasterPos2f(textStartX, textStartY);
    for (char c : startText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glfwSwapBuffers(window);

    //等待鼠标点击开始游戏
    while (true) {
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            // 转换坐标到 -1到1范围
            x = (x / 400.0) - 1.0;
            y = -(y / 400.0) + 1.0;
            // 检查是否点击了按钮区域
            if (x >= -0.5f && x <= 0.5f && y >= -0.2f && y <= 0.2f) {
                break;
            }
        }
    }
}


void showEndScreen(GLFWwindow* window, bool isWon) {
    //获取窗口宽度和高度
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    //设置背景颜色
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::string endText;
    if (isWon) {
        endText = "congratulations！";
    }
    else {
        endText = "Run out of time...";
    }

    glColor3f(0.0f, 0.0f, 0.0f);

    //计算文本宽度
    double textWidth = 0.0;
    for (char c : endText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //计算文本高度
    double textHeight = 24;

    //计算文本在窗口中的居中位置
    double textX = (windowWidth / 2.0 - textWidth / 2.0) / 400.0 - 1.0;
    double textY = (windowHeight / 2.0 - textHeight / 2.0) / 400.0 - 1.0;

    //设置文本位置并绘制
    glRasterPos2f(textX, textY);
    for (char c : endText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glfwSwapBuffers(window);

    //等待3秒后关闭窗口
    std::this_thread::sleep_for(std::chrono::seconds(3));
    glfwSetWindowShouldClose(window, GL_TRUE);
}