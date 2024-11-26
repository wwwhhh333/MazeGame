#include "MazeGame.h"

const int MAZE_WIDTH = 40;
const int MAZE_HEIGHT = 40;
const float CELL_SIZE = 2.0 / MAZE_WIDTH;
const int TIME_LIMIT = 60;

//随机Prim算法初始化迷宫
void MazeGame::initMaze() {
    //让迷宫全是墙  
    for (int y = 0; y < MAZE_HEIGHT-3; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze[y][x] = WALL;
        }
    }

    int dx[] = { 0, 1, 0, -1 };
    int dy[] = { 1, 0, -1, 0 };

    
    std::vector<std::pair<int, int>> walls; //墙壁列表  

    //选择起始单元格（例如迷宫中心）  
    int startX = MAZE_WIDTH / 2;
    int startY = MAZE_HEIGHT / 2;
    maze[startY][startX] = PATH;

    //将起始单元格的邻墙加入列表
    for (int i = 0; i < 4; i++) {
        int wallX = startX + dx[i];
        int wallY = startY + dy[i];
        if (wallX >= 0 && wallX < MAZE_WIDTH &&
            wallY >= 0 && wallY < MAZE_HEIGHT-3) {
            walls.push_back({ wallX, wallY });
        }
    }

    //当列表里还有墙时  
    while (!walls.empty()) {
        //随机选择一面墙  
        int wallIndex = rand() % walls.size();
        int wallX = walls[wallIndex].first;
        int wallY = walls[wallIndex].second;

        //计算墙两侧的单元格
        int visitedCount = 0;
        std::vector<std::pair<int, int>> cells;

        //如果是竖向墙 
        if(maze[wallY][wallX-1]==PATH|| maze[wallY][wallX + 1] == PATH)
        {
            for (int i = -1; i <= 1; i += 2) {
                int cellX = wallX + i;
                int cellY = wallY;

                if (cellX > 0 && cellX < MAZE_WIDTH - 1 &&
                    cellY > 0 && cellY < MAZE_HEIGHT - 1) {
                    cells.push_back({ cellX, cellY });
                    if (maze[cellY][cellX] == PATH) {
                        visitedCount++;
                    }
                }
            } 
        }

        //如果是横向墙 
        else if (maze[wallY-1][wallX] == PATH || maze[wallY+1][wallX] == PATH)
        {
            for (int i = -1; i <= 1; i += 2) {
                int cellX = wallX;
                int cellY = wallY + i;

                if (cellX > 0 && cellX < MAZE_WIDTH - 1 &&
                    cellY > 0 && cellY < MAZE_HEIGHT - 1) {
                    cells.push_back({ cellX, cellY });
                    if (maze[cellY][cellX] == PATH) {
                        visitedCount++;
                    }
                }
            }
        }

        //如果只有一个单元格被访问，打通墙 
        if (visitedCount == 1) {
            maze[wallY][wallX] = PATH;

            //找到未访问的单元格并标记为路径
            for (auto& cell : cells) {
                if (maze[cell.second][cell.first] != PATH) {
                    maze[cell.second][cell.first] = PATH;

                    //将新单元格的邻墙加入列表  
                    for (int i = 0; i < 4; i++) {
                        int newWallX = cell.first + dx[i];
                        int newWallY = cell.second + dy[i];

                        //确保新墙在迷宫范围内且是墙  
                        if (newWallX > 0 && newWallX < MAZE_WIDTH - 1 &&
                            newWallY > 0 && newWallY < MAZE_HEIGHT - 3 &&
                            maze[newWallY][newWallX] == WALL) {
                            walls.push_back({ newWallX, newWallY });
                        }
                    }
                    break;
                }
            }
        } //if

        walls.erase(walls.begin() + wallIndex); //从墙列表中移除当前墙  
    } //while

    //设置老鼠和目标位置  
    mouseX = MAZE_WIDTH / 2;
    mouseY = MAZE_HEIGHT / 2;
    maze[mouseY][mouseX] = MOUSE;

    targetX = MAZE_WIDTH - 2; targetY = 2;
    while(1) {
        if (maze[targetY][targetX] == PATH) {
            maze[targetY][targetX] = TARGET;
            break;
        }
        else {
            targetY++;
        }
    }
}

//绘制提示栏
void MazeGame::drawPromptBar() {
    //绘制提示栏背景
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.9f);
    glVertex2f(1.0f, 0.9f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //绘制左上方提示文字
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.95f, 0.92f);
    std::string Text = "Press e to enter EditMode       Press t to get a hint";
    for (char c : Text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //绘制右上方剩余时间
    std::string timeText = "Remaining Time: " + std::to_string(TIME_LIMIT - getElapsedTime());
    glRasterPos2f(0.4f, 0.92f);
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

//绘制迷宫
void MazeGame::drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓冲区

    for (int y = 0; y < MAZE_HEIGHT-2; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            //计算当前单元格在OpenGL坐标系中坐标(-1.0f, 1.0f)
            float posX = x * CELL_SIZE - 1.0f;
            float posY = y * CELL_SIZE - 1.0f;

            //根据单元格类型设置颜色
            switch (maze[y][x]) {
            case WALL: //灰/红 
                glColor3f(editMode ? 1.0f : 0.5f, editMode ? 0.0f : 0.5f, editMode ? 0.0f : 0.5f);
                break;
            case PATH: //白
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            case MOUSE: //绿
                glColor3f( 0.0f, 1.0f, 0.0f);
                break;
            case TARGET: //品红
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            case TIPS1: //青色
                glColor3f(0.0f, 1.0f, 1.0f);
                break;
            case TIPS2: //黄色
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
            }

            //绘制单元格
            glBegin(GL_QUADS);
            glVertex2f(posX, posY);
            glVertex2f(posX + CELL_SIZE, posY);
            glVertex2f(posX + CELL_SIZE, posY + CELL_SIZE);
            glVertex2f(posX, posY + CELL_SIZE);
            glEnd();
        }
    }

    drawPromptBar(); //绘制提示栏
}

//绘制路径
void MazeGame::drawPath() {
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << "绘制所有路径" << std::endl;
    for (const auto& path : allPaths) {
        for (const auto& point : path) {
            if (point.first == mouseX && point.second == mouseY
                || point.first == targetX && point.second == targetY)
                continue;
            maze[point.second][point.first] = TIPS1;
        }
    }


    std::cout << "绘制最短路径" << std::endl;
    for (const auto& path : shortestPath) {
        if (path.first == mouseX&&path.second==mouseY 
            || path.first == targetX && path.second == targetY) 
            continue;

        maze[path.second][path.first] = TIPS2;
    }
}

//移动老鼠
void MazeGame::moveMouse(int dx, int dy) {
    //计算老鼠移动后的新坐标
    int newX = mouseX + dx;
    int newY = mouseY + dy;

    //检查新位置是否在迷宫范围内且不是墙壁
    if (newX >= 0 && newX < MAZE_WIDTH &&
        newY >= 0 && newY < MAZE_HEIGHT &&
        maze[newY][newX] != WALL) {

        maze[mouseY][mouseX] = PATH; //将老鼠原来所在位置设置为路径

        //更新老鼠坐标
        mouseX = newX;
        mouseY = newY;

        maze[mouseY][mouseX] = MOUSE; //将老鼠新位置设置为MOUSE类型
    }
}

//检查游戏状态
void MazeGame::checkGameStatus() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    //如果经过的时间超过了时间限制
    if (elapsedTime > TIME_LIMIT) {
        gameOver = true;
        gameWon = false;
    }

    //如果老鼠到达了目标位置
    else if (mouseX == targetX && mouseY == targetY) {
        gameOver = true;
        gameWon = true;
    }
}

//寻找所有从老鼠当前位置到目标位置路径
void MazeGame::findAllPathDFS() {
    allPaths.clear(); //重置所有路径

    DFS(mouseX, mouseY, {});

    int shortestPathLength = INT_MAX;

    //遍历所有找到的路径，找到最短路径
    for (const auto& path : allPaths) {
        if (path.size() < shortestPathLength) {
            shortestPathLength = path.size();
            shortestPath = path;
        }
    }

    std::cout << "总共找到 " << allPaths.size() << " 条路径:" << std::endl;
    for (const auto& path : allPaths) {
        std::cout << "路径：";
        for (const auto& point : path) {
            std::cout << "(" << point.first << "," << point.second << ") ";
        }
        std::cout << std::endl << std::endl;
    }

    //输出最短路径信息
    if (!shortestPath.empty()) {
        std::cout << "最短路径：";
        for (const auto& point : shortestPath) {
            std::cout << "(" << point.first << "," << point.second << ") ";
        }
        std::cout << std::endl;
        std::cout << "长度为：" << shortestPath.size() << std::endl << std::endl;
    }
    else {
        std::cout << "未找到通往目标的路径。" << std::endl;
    }

    drawPath();
}

//深度优先搜索
void MazeGame::DFS(int currentX, int currentY, std::vector<std::pair<int, int>> currentPath) {
    
    currentPath.push_back({ currentX, currentY }); //将当前位置加入路径

    //已到达目标点，保存完整路径
    if (currentX == targetX && currentY == targetY) {
        allPaths.push_back(currentPath);
        return;
    }

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { 1, -1, 0, 0 };

    for (int i = 0; i < 4; i++) {
        int nextX = currentX + dx[i];
        int nextY = currentY + dy[i];
        
        if (isValidPosition(nextX, nextY, currentPath)) {
            DFS(nextX, nextY, currentPath);
        }
    }
}

//判断位置是否合法
bool MazeGame::isValidPosition(int nextX, int nextY, const std::vector<std::pair<int, int>>& currentPath) {
    
    //是否在迷宫范围内
    if (nextX <= 0 || nextX >= MAZE_WIDTH || nextY <= 0 || nextY >= MAZE_HEIGHT) {
        return false;
    }

    //是否不是墙壁
    if (maze[nextY][nextX] == WALL) {
        return false;
    }

    //没有被访问过
    if (std::find_if(currentPath.begin(), currentPath.end(),
        [&](const std::pair<int, int>& p) {
            return p.first == nextX && p.second == nextY;
        }) != currentPath.end()) {
        return false;
    }

    return true;
}

//迷宫游戏类构造函数
MazeGame::MazeGame() {
    initMaze(); //初始化迷宫布局
    startTime = std::chrono::steady_clock::now(); //记录游戏开始的时间

    gameOver = false;
    gameWon = false;
    editMode = false;
}

//处理按键按下事件
void MazeGame::handleKeyPress(int key) {
    if (gameOver) return;

        switch (key) { //根据按下的按键来移动老鼠
        case GLFW_KEY_UP:
            if (!editMode) moveMouse(0, 1);
            break;
        case GLFW_KEY_DOWN:
            if (!editMode) moveMouse(0, -1);
            break;
        case GLFW_KEY_LEFT:
            if (!editMode) moveMouse(-1, 0);
            break;
        case GLFW_KEY_RIGHT:
            if (!editMode) moveMouse(1, 0);
            break;
        case GLFW_KEY_E: //切换编辑模式
            editMode = !editMode;
            std::cout << (editMode ? "进入编辑模式" : "退出编辑模式") << std::endl;
            if (editMode) {
                pausedStartTime = std::chrono::steady_clock::now(); //暂停时间
            }
            else {
                startTime += std::chrono::steady_clock::now() - pausedStartTime; //恢复时间
            }
            break;
        case GLFW_KEY_T:
            if (!editMode) findAllPathDFS();
            break;
        }
   
    checkGameStatus(); //检查游戏状态
}

//编辑模式下点击切换墙和路
void MazeGame::toggleCell(int x, int y) {
    std::cout << "切换前单元格(" << x << ", " << y << ")类型为: " << (maze[y][x] == WALL ? "WALL" : "PATH") << std::endl;
    if (x > 0 && x < MAZE_WIDTH - 1 && y > 0 && y < MAZE_HEIGHT - 3) {
        if (maze[y][x] == WALL) maze[y][x] = PATH;
        else if (maze[y][x] == PATH) maze[y][x] = WALL;
    }
    std::cout << "切换后单元格(" << x << ", " << y << ")类型为: " << (maze[y][x] == WALL ? "WALL" : "PATH") << std::endl;
}

//处理鼠标点击事件
void MazeGame::handleMouseClick(double x, double y) {
    if (editMode) {
        //将openGL坐标转换为迷宫坐标  
        int mazeX = static_cast<int>((x + 1.0) / CELL_SIZE);
        int mazeY = static_cast<int>((y + 1.0) / CELL_SIZE);
        toggleCell(mazeX, mazeY);
    }
}

//渲染游戏画面，主要是绘制迷宫
void MazeGame::render() {
    drawMaze();
}

//判断游戏是否结束
bool MazeGame::isGameOver() { return gameOver; }

//判断游戏是否胜利
bool MazeGame::isGameWon() { return gameWon; }

//获取游戏运行时间
int MazeGame::getElapsedTime() {
    if (editMode) {
        //返回进入编辑模式时已经过去的时间
        return std::chrono::duration_cast<std::chrono::seconds>(pausedStartTime - startTime).count();
    }
    else {
        //计算从游戏开始到现在经过的时间
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
    }
}