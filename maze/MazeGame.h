#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <chrono>
#include <ctime>
#include <string>

enum CellType { WALL, PATH, MOUSE, TARGET, TIPS1, TIPS2 }; //��Ԫ������
struct Color { float r, g, b; };

//�Թ���Ϸ��
class MazeGame {
private:
    CellType maze[100][100]; //�洢�Թ����ֵĶ�ά����
    int mouseX, mouseY; //����ǰ����
    int targetX, targetY; //Ŀ��λ������
    std::chrono::steady_clock::time_point startTime; //��Ϸ��ʼ��ʱ��
    std::chrono::steady_clock::time_point pausedStartTime; //��¼��ͣ��ʱ��
    bool gameOver;  //��Ϸ�Ƿ�����ı�־
    bool gameWon; //��Ϸ�Ƿ�ʤ���ı�־
    bool editMode; //�Ƿ��ڱ༭ģʽ

    void initMaze(); //��ʼ���Թ�
    void drawPromptBar();
    void drawMaze(); //�����Թ�
    void moveMouse(int dx, int dy); //�ƶ�����
    void checkGameStatus(); //�����Ϸ״̬

    std::vector<std::vector<std::pair<int, int>>> allPaths; //ȫ��·��
    std::vector<std::pair<int, int>> shortestPath; //���·��
    void DFS(int currentX, int currentY, std::vector<std::pair<int, int>> currentPath);
    bool isValidPosition(int nextX, int nextY, const std::vector<std::pair<int, int>>& currentPath);
    void findAllPathDFS(); //Ѱ������·��
    void drawPath(); //����·��

    void toggleCell(int x, int y); //�༭ģʽ�µ���л�ǽ��·
    int getElapsedTime(); //��ȡ��Ϸ����ʱ��

public:
    MazeGame(); //�Թ���Ϸ�๹�캯��
    void handleKeyPress(int key); //�����������¼�
    void handleMouseClick(double x, double y); //����������¼�
    void render(); //��Ⱦ��Ϸ����
    bool isGameOver(); //�ж���Ϸ�Ƿ����
    bool isGameWon(); //�ж���Ϸ�Ƿ�ʤ��
};