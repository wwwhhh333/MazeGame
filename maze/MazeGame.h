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

enum CellType { WALL, PATH, MOUSE, TARGET, TIPS1, TIPS2 }; //单元格类型
struct Color { float r, g, b; };

//迷宫游戏类
class MazeGame {
private:
    CellType maze[100][100]; //存储迷宫布局的二维数组
    int mouseX, mouseY; //老鼠当前坐标
    int targetX, targetY; //目标位置坐标
    std::chrono::steady_clock::time_point startTime; //游戏开始的时间
    std::chrono::steady_clock::time_point pausedStartTime; //记录暂停的时间
    bool gameOver;  //游戏是否结束的标志
    bool gameWon; //游戏是否胜利的标志
    bool editMode; //是否处于编辑模式

    void initMaze(); //初始化迷宫
    void drawPromptBar();
    void drawMaze(); //绘制迷宫
    void moveMouse(int dx, int dy); //移动老鼠
    void checkGameStatus(); //检查游戏状态

    std::vector<std::vector<std::pair<int, int>>> allPaths; //全部路径
    std::vector<std::pair<int, int>> shortestPath; //最短路径
    void DFS(int currentX, int currentY, std::vector<std::pair<int, int>> currentPath);
    bool isValidPosition(int nextX, int nextY, const std::vector<std::pair<int, int>>& currentPath);
    void findAllPathDFS(); //寻找所有路径
    void drawPath(); //绘制路径

    void toggleCell(int x, int y); //编辑模式下点击切换墙和路
    int getElapsedTime(); //获取游戏运行时间

public:
    MazeGame(); //迷宫游戏类构造函数
    void handleKeyPress(int key); //处理按键按下事件
    void handleMouseClick(double x, double y); //处理鼠标点击事件
    void render(); //渲染游戏画面
    bool isGameOver(); //判断游戏是否结束
    bool isGameWon(); //判断游戏是否胜利
};