#include "MazeGame.h"
#include "Screen.h"

//按键回调函数，当有按键按下时被GLFW调用
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) { 
        MazeGame* game = static_cast<MazeGame*>(glfwGetWindowUserPointer(window)); //获取与窗口关联的游戏对象指针
        game->handleKeyPress(key); //调用游戏对象的按键处理函数
    }
}

//点击回调函数，当鼠标点击时被GLFW调用
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    //std::cout << "mouseButtonCallback函数被调用" << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        //转换OpenGL坐标系
        x = (x / 400.0) - 1.0;
        y = -(y / 400.0) + 1.0;

        MazeGame* game = static_cast<MazeGame*>(glfwGetWindowUserPointer(window));
        game->handleMouseClick(x, y);
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 800, "Maze", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); //将创建的窗口设置为当前OpenGL上下文
    glewInit();

    //显示开始界面
    showStartScreen(window);

    MazeGame game;

    glfwSetWindowUserPointer(window, &game); //将游戏对象与窗口关联起来，以便在按键回调函数中能获取到游戏对象
    glfwSetKeyCallback(window, keyCallback);  //设置按键回调函数，当有按键按下时会调用keyCallback函数
    glfwSetMouseButtonCallback(window, mouseButtonCallback); //设置点击回调函数，当鼠标点击时会调用mouseButtonCallback函数
     
    //主循环
    while (!glfwWindowShouldClose(window)) {
        game.render(); //渲染游戏画面
        glfwSwapBuffers(window);
        glfwPollEvents(); //处理窗口事件，如按键、鼠标移动等

        if (game.isGameOver()) {
            showEndScreen(window, game.isGameWon()); //显示结果界面
            std::cout << (game.isGameWon() ? "恭喜，成功到达粮仓！" : "很遗憾，时间用完了。") << std::endl;
            break;
        }
    }

    glfwTerminate();
    return 0;
}