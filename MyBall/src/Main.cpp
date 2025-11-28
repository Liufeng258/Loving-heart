#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

struct Ball {
    int x, y;
    int radius;
    char symbol;
};

Ball player = {WIDTH / 2, HEIGHT / 2, 1, 'O'};
Ball foods[5] = {};
Ball enemies[3] = {};

void initFoods() {
    for (int i = 0; i < 5; ++i) {
        foods[i].x = rand() % WIDTH;
        foods[i].y = rand() % HEIGHT;
        foods[i].radius = 1;
        foods[i].symbol = '*';
    }
}

void initEnemies() {
    for (int i = 0; i < 3; ++i) {
        enemies[i].x = rand() % WIDTH;
        enemies[i].y = rand() % HEIGHT;
        enemies[i].radius = 2;
        enemies[i].symbol = 'X';
    }
}

void drawMap() {
    system("cls");
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            bool drawn = false;
            if (player.x == j && player.y == i) {
                cout << player.symbol;
                drawn = true;
            } else {
                for (int k = 0; k < 5; ++k) {
                    if (foods[k].x == j && foods[k].y == i) {
                        cout << foods[k].symbol;
                        drawn = true;
                        break;
                    }
                }
                if (!drawn) {
                    for (int k = 0; k < 3; ++k) {
                        if (enemies[k].x == j && enemies[k].y == i) {
                            cout << enemies[k].symbol;
                            drawn = true;
                            break;
                        }
                    }
                }
                if (!drawn) cout << ' ';
            }
        }
        cout << endl;
    }
}

void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
        case 'w':
        case 'W':
            if (player.y > 0) player.y--;
            break;
        case 's':
        case 'S':
            if (player.y < HEIGHT - 1) player.y++;
            break;
        case 'a':
        case 'A':
            if (player.x > 0) player.x--;
            break;
        case 'd':
        case 'D':
            if (player.x < WIDTH - 1) player.x++;
            break;
        }
    }
}

void updateGame() {
    // 简单敌人AI：向玩家移动
    for (int i = 0; i < 3; ++i) {
        if (enemies[i].x < player.x) enemies[i].x++;
        else if (enemies[i].x > player.x) enemies[i].x--;
        if (enemies[i].y < player.y) enemies[i].y++;
        else if (enemies[i].y > player.y) enemies[i].y--;
    }

    // 检查玩家与食物碰撞
    for (int i = 0; i < 5; ++i) {
        if (player.x == foods[i].x && player.y == foods[i].y) {
            player.radius++;
            foods[i].x = rand() % WIDTH;
            foods[i].y = rand() % HEIGHT;
        }
    }

    // 检查玩家与敌人碰撞
    for (int i = 0; i < 3; ++i) {
        if (player.x == enemies[i].x && player.y == enemies[i].y && enemies[i].radius > player.radius) {
            cout << "游戏结束！你被吃掉了！\n";
            Sleep(2000);
            exit(0);
        }
    }
}

int main() {
    srand((unsigned)time(0));
    initFoods();
    initEnemies();

    while (true) {
        handleInput();
        updateGame();
        drawMap();
        Sleep(100); // 控制刷新速度
    }

    return 0;
}
