#include "MazeGame.h"

const int MAZE_WIDTH = 40;
const int MAZE_HEIGHT = 40;
const float CELL_SIZE = 2.0 / MAZE_WIDTH;
const int TIME_LIMIT = 60;

//���Prim�㷨��ʼ���Թ�
void MazeGame::initMaze() {
    //���Թ�ȫ��ǽ  
    for (int y = 0; y < MAZE_HEIGHT-3; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze[y][x] = WALL;
        }
    }

    int dx[] = { 0, 1, 0, -1 };
    int dy[] = { 1, 0, -1, 0 };

    
    std::vector<std::pair<int, int>> walls; //ǽ���б�  

    //ѡ����ʼ��Ԫ�������Թ����ģ�  
    int startX = MAZE_WIDTH / 2;
    int startY = MAZE_HEIGHT / 2;
    maze[startY][startX] = PATH;

    //����ʼ��Ԫ�����ǽ�����б�
    for (int i = 0; i < 4; i++) {
        int wallX = startX + dx[i];
        int wallY = startY + dy[i];
        if (wallX >= 0 && wallX < MAZE_WIDTH &&
            wallY >= 0 && wallY < MAZE_HEIGHT-3) {
            walls.push_back({ wallX, wallY });
        }
    }

    //���б��ﻹ��ǽʱ  
    while (!walls.empty()) {
        //���ѡ��һ��ǽ  
        int wallIndex = rand() % walls.size();
        int wallX = walls[wallIndex].first;
        int wallY = walls[wallIndex].second;

        //����ǽ����ĵ�Ԫ��
        int visitedCount = 0;
        std::vector<std::pair<int, int>> cells;

        //���������ǽ 
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

        //����Ǻ���ǽ 
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

        //���ֻ��һ����Ԫ�񱻷��ʣ���ͨǽ 
        if (visitedCount == 1) {
            maze[wallY][wallX] = PATH;

            //�ҵ�δ���ʵĵ�Ԫ�񲢱��Ϊ·��
            for (auto& cell : cells) {
                if (maze[cell.second][cell.first] != PATH) {
                    maze[cell.second][cell.first] = PATH;

                    //���µ�Ԫ�����ǽ�����б�  
                    for (int i = 0; i < 4; i++) {
                        int newWallX = cell.first + dx[i];
                        int newWallY = cell.second + dy[i];

                        //ȷ����ǽ���Թ���Χ������ǽ  
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

        walls.erase(walls.begin() + wallIndex); //��ǽ�б����Ƴ���ǰǽ  
    } //while

    //���������Ŀ��λ��  
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

//������ʾ��
void MazeGame::drawPromptBar() {
    //������ʾ������
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.9f);
    glVertex2f(1.0f, 0.9f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    //�������Ϸ���ʾ����
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.95f, 0.92f);
    std::string Text = "Press e to enter EditMode       Press t to get a hint";
    for (char c : Text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    //�������Ϸ�ʣ��ʱ��
    std::string timeText = "Remaining Time: " + std::to_string(TIME_LIMIT - getElapsedTime());
    glRasterPos2f(0.4f, 0.92f);
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

//�����Թ�
void MazeGame::drawMaze() {
    glClear(GL_COLOR_BUFFER_BIT); //�����ɫ������

    for (int y = 0; y < MAZE_HEIGHT-2; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            //���㵱ǰ��Ԫ����OpenGL����ϵ������(-1.0f, 1.0f)
            float posX = x * CELL_SIZE - 1.0f;
            float posY = y * CELL_SIZE - 1.0f;

            //���ݵ�Ԫ������������ɫ
            switch (maze[y][x]) {
            case WALL: //��/�� 
                glColor3f(editMode ? 1.0f : 0.5f, editMode ? 0.0f : 0.5f, editMode ? 0.0f : 0.5f);
                break;
            case PATH: //��
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            case MOUSE: //��
                glColor3f( 0.0f, 1.0f, 0.0f);
                break;
            case TARGET: //Ʒ��
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            case TIPS1: //��ɫ
                glColor3f(0.0f, 1.0f, 1.0f);
                break;
            case TIPS2: //��ɫ
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
            }

            //���Ƶ�Ԫ��
            glBegin(GL_QUADS);
            glVertex2f(posX, posY);
            glVertex2f(posX + CELL_SIZE, posY);
            glVertex2f(posX + CELL_SIZE, posY + CELL_SIZE);
            glVertex2f(posX, posY + CELL_SIZE);
            glEnd();
        }
    }

    drawPromptBar(); //������ʾ��
}

//����·��
void MazeGame::drawPath() {
    glClear(GL_COLOR_BUFFER_BIT);

    std::cout << "��������·��" << std::endl;
    for (const auto& path : allPaths) {
        for (const auto& point : path) {
            if (point.first == mouseX && point.second == mouseY
                || point.first == targetX && point.second == targetY)
                continue;
            maze[point.second][point.first] = TIPS1;
        }
    }


    std::cout << "�������·��" << std::endl;
    for (const auto& path : shortestPath) {
        if (path.first == mouseX&&path.second==mouseY 
            || path.first == targetX && path.second == targetY) 
            continue;

        maze[path.second][path.first] = TIPS2;
    }
}

//�ƶ�����
void MazeGame::moveMouse(int dx, int dy) {
    //���������ƶ����������
    int newX = mouseX + dx;
    int newY = mouseY + dy;

    //�����λ���Ƿ����Թ���Χ���Ҳ���ǽ��
    if (newX >= 0 && newX < MAZE_WIDTH &&
        newY >= 0 && newY < MAZE_HEIGHT &&
        maze[newY][newX] != WALL) {

        maze[mouseY][mouseX] = PATH; //������ԭ������λ������Ϊ·��

        //������������
        mouseX = newX;
        mouseY = newY;

        maze[mouseY][mouseX] = MOUSE; //��������λ������ΪMOUSE����
    }
}

//�����Ϸ״̬
void MazeGame::checkGameStatus() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    //���������ʱ�䳬����ʱ������
    if (elapsedTime > TIME_LIMIT) {
        gameOver = true;
        gameWon = false;
    }

    //������󵽴���Ŀ��λ��
    else if (mouseX == targetX && mouseY == targetY) {
        gameOver = true;
        gameWon = true;
    }
}

//Ѱ�����д�����ǰλ�õ�Ŀ��λ��·��
void MazeGame::findAllPathDFS() {
    allPaths.clear(); //��������·��

    DFS(mouseX, mouseY, {});

    int shortestPathLength = INT_MAX;

    //���������ҵ���·�����ҵ����·��
    for (const auto& path : allPaths) {
        if (path.size() < shortestPathLength) {
            shortestPathLength = path.size();
            shortestPath = path;
        }
    }

    std::cout << "�ܹ��ҵ� " << allPaths.size() << " ��·��:" << std::endl;
    for (const auto& path : allPaths) {
        std::cout << "·����";
        for (const auto& point : path) {
            std::cout << "(" << point.first << "," << point.second << ") ";
        }
        std::cout << std::endl << std::endl;
    }

    //������·����Ϣ
    if (!shortestPath.empty()) {
        std::cout << "���·����";
        for (const auto& point : shortestPath) {
            std::cout << "(" << point.first << "," << point.second << ") ";
        }
        std::cout << std::endl;
        std::cout << "����Ϊ��" << shortestPath.size() << std::endl << std::endl;
    }
    else {
        std::cout << "δ�ҵ�ͨ��Ŀ���·����" << std::endl;
    }

    drawPath();
}

//�����������
void MazeGame::DFS(int currentX, int currentY, std::vector<std::pair<int, int>> currentPath) {
    
    currentPath.push_back({ currentX, currentY }); //����ǰλ�ü���·��

    //�ѵ���Ŀ��㣬��������·��
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

//�ж�λ���Ƿ�Ϸ�
bool MazeGame::isValidPosition(int nextX, int nextY, const std::vector<std::pair<int, int>>& currentPath) {
    
    //�Ƿ����Թ���Χ��
    if (nextX <= 0 || nextX >= MAZE_WIDTH || nextY <= 0 || nextY >= MAZE_HEIGHT) {
        return false;
    }

    //�Ƿ���ǽ��
    if (maze[nextY][nextX] == WALL) {
        return false;
    }

    //û�б����ʹ�
    if (std::find_if(currentPath.begin(), currentPath.end(),
        [&](const std::pair<int, int>& p) {
            return p.first == nextX && p.second == nextY;
        }) != currentPath.end()) {
        return false;
    }

    return true;
}

//�Թ���Ϸ�๹�캯��
MazeGame::MazeGame() {
    initMaze(); //��ʼ���Թ�����
    startTime = std::chrono::steady_clock::now(); //��¼��Ϸ��ʼ��ʱ��

    gameOver = false;
    gameWon = false;
    editMode = false;
}

//�����������¼�
void MazeGame::handleKeyPress(int key) {
    if (gameOver) return;

        switch (key) { //���ݰ��µİ������ƶ�����
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
        case GLFW_KEY_E: //�л��༭ģʽ
            editMode = !editMode;
            std::cout << (editMode ? "����༭ģʽ" : "�˳��༭ģʽ") << std::endl;
            if (editMode) {
                pausedStartTime = std::chrono::steady_clock::now(); //��ͣʱ��
            }
            else {
                startTime += std::chrono::steady_clock::now() - pausedStartTime; //�ָ�ʱ��
            }
            break;
        case GLFW_KEY_T:
            if (!editMode) findAllPathDFS();
            break;
        }
   
    checkGameStatus(); //�����Ϸ״̬
}

//�༭ģʽ�µ���л�ǽ��·
void MazeGame::toggleCell(int x, int y) {
    std::cout << "�л�ǰ��Ԫ��(" << x << ", " << y << ")����Ϊ: " << (maze[y][x] == WALL ? "WALL" : "PATH") << std::endl;
    if (x > 0 && x < MAZE_WIDTH - 1 && y > 0 && y < MAZE_HEIGHT - 3) {
        if (maze[y][x] == WALL) maze[y][x] = PATH;
        else if (maze[y][x] == PATH) maze[y][x] = WALL;
    }
    std::cout << "�л���Ԫ��(" << x << ", " << y << ")����Ϊ: " << (maze[y][x] == WALL ? "WALL" : "PATH") << std::endl;
}

//����������¼�
void MazeGame::handleMouseClick(double x, double y) {
    if (editMode) {
        //��openGL����ת��Ϊ�Թ�����  
        int mazeX = static_cast<int>((x + 1.0) / CELL_SIZE);
        int mazeY = static_cast<int>((y + 1.0) / CELL_SIZE);
        toggleCell(mazeX, mazeY);
    }
}

//��Ⱦ��Ϸ���棬��Ҫ�ǻ����Թ�
void MazeGame::render() {
    drawMaze();
}

//�ж���Ϸ�Ƿ����
bool MazeGame::isGameOver() { return gameOver; }

//�ж���Ϸ�Ƿ�ʤ��
bool MazeGame::isGameWon() { return gameWon; }

//��ȡ��Ϸ����ʱ��
int MazeGame::getElapsedTime() {
    if (editMode) {
        //���ؽ���༭ģʽʱ�Ѿ���ȥ��ʱ��
        return std::chrono::duration_cast<std::chrono::seconds>(pausedStartTime - startTime).count();
    }
    else {
        //�������Ϸ��ʼ�����ھ�����ʱ��
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
    }
}