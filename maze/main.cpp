#include "MazeGame.h"
#include "Screen.h"

//�����ص����������а�������ʱ��GLFW����
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) { 
        MazeGame* game = static_cast<MazeGame*>(glfwGetWindowUserPointer(window)); //��ȡ�봰�ڹ�������Ϸ����ָ��
        game->handleKeyPress(key); //������Ϸ����İ���������
    }
}

//����ص��������������ʱ��GLFW����
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    //std::cout << "mouseButtonCallback����������" << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        //ת��OpenGL����ϵ
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

    glfwMakeContextCurrent(window); //�������Ĵ�������Ϊ��ǰOpenGL������
    glewInit();

    //��ʾ��ʼ����
    showStartScreen(window);

    MazeGame game;

    glfwSetWindowUserPointer(window, &game); //����Ϸ�����봰�ڹ����������Ա��ڰ����ص��������ܻ�ȡ����Ϸ����
    glfwSetKeyCallback(window, keyCallback);  //���ð����ص����������а�������ʱ�����keyCallback����
    glfwSetMouseButtonCallback(window, mouseButtonCallback); //���õ���ص��������������ʱ�����mouseButtonCallback����
     
    //��ѭ��
    while (!glfwWindowShouldClose(window)) {
        game.render(); //��Ⱦ��Ϸ����
        glfwSwapBuffers(window);
        glfwPollEvents(); //�������¼����簴��������ƶ���

        if (game.isGameOver()) {
            showEndScreen(window, game.isGameWon()); //��ʾ�������
            std::cout << (game.isGameWon() ? "��ϲ���ɹ��������֣�" : "���ź���ʱ�������ˡ�") << std::endl;
            break;
        }
    }

    glfwTerminate();
    return 0;
}