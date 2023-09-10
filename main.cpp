#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <graphics.h>

// Kích thu?c mê cung
#define ROWS 10
#define COLS 10

// Kích thu?c ô vuông
#define SQUARE_SIZE 40

// C?u trúc d? luu t?a d? c?a m?t ô trong mê cung
struct Point {
    int x, y;
};

// Hàm ki?m tra xem ô có n?m trong mê cung và không ph?i là tu?ng hay dã du?c duy?t chua
bool isValid(int maze[ROWS][COLS], int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0);
}
// Hàm tính kho?ng cách Manhattan gi?a hai di?m
int manhattanDistance(struct Point p1, struct Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

// Hàm th?c hi?n thu?t toán A*
void aStar(int maze[ROWS][COLS], struct Point start, struct Point end, struct Point parent[ROWS][COLS], int cost[ROWS][COLS]) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    // Hàng d?i uu tiên dùng d? luu các ô chua duy?t theo th? t? uu tiên c?a A*
    struct Point openList[ROWS * COLS];
    int openListSize = 0;

    // Danh sách luu l?i các ô dã duy?t
    bool closedList[ROWS][COLS] = {false};

    // Kh?i t?o giá tr? cost c?a di?m b?t d?u là 0
    cost[start.x][start.y] = 0;

    // Thêm di?m b?t d?u vào hàng d?i uu tiên
    openList[openListSize++] = start;

    while (openListSize > 0) {
        // Tìm ô có u?c lu?ng chi phí nh? nh?t trong hàng d?i uu tiên
        int minCost = INT_MAX;
        int minIndex = 0;
        for (int i = 0; i < openListSize; i++) {
            int x = openList[i].x;
            int y = openList[i].y;
            if (cost[x][y] + manhattanDistance(openList[i], end) < minCost) {
                minCost = cost[x][y] + manhattanDistance(openList[i], end);
                minIndex = i;
            }
        }

        struct Point current = openList[minIndex];

        // Di chuy?n ô có u?c lu?ng chi phí nh? nh?t ra kh?i hàng d?i uu tiên
        for (int i = minIndex; i < openListSize - 1; i++) {
            openList[i] = openList[i + 1];
        }
        openListSize--;

        // Ðánh d?u ô hi?n t?i là dã duy?t
        closedList[current.x][current.y] = true;

        // N?u tìm th?y ô dích, thoát kh?i vòng l?p
        if (current.x == end.x && current.y == end.y) {
            break;
        }

        // Duy?t qua các ô lân c?n
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (isValid(maze, newX, newY) && !closedList[newX][newY]) {
                int newCost = cost[current.x][current.y] + 1;

                if (newCost < cost[newX][newY]) {
                    cost[newX][newY] = newCost;
                    parent[newX][newY] = current;
                    openList[openListSize++] = (struct Point){newX, newY};
                }
            }
        }
    }
}
// Hàm v? l? trình tìm du?c t? A*
void drawPath(struct Point parent[ROWS][COLS], struct Point start, struct Point end) {
    int x = end.y * SQUARE_SIZE + SQUARE_SIZE / 2;
    int y = end.x * SQUARE_SIZE + SQUARE_SIZE / 2;

    while (end.x != start.x || end.y != start.y) {
        int newX = parent[end.x][end.y].x;
        int newY = parent[end.x][end.y].y;

        line(end.y * SQUARE_SIZE + SQUARE_SIZE / 2, end.x * SQUARE_SIZE + SQUARE_SIZE / 2,
             newY * SQUARE_SIZE + SQUARE_SIZE / 2, newX * SQUARE_SIZE + SQUARE_SIZE / 2);
        end.x = newX;
        end.y = newY;
    }

    circle(x, y, 10);
}
void drawMaze(int maze[ROWS][COLS]) {
    int x, y;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            x = j * SQUARE_SIZE;
            y = i * SQUARE_SIZE;

            if (maze[i][j] == 1) {
                rectangle(x, y, x + SQUARE_SIZE, y + SQUARE_SIZE);
                floodfill(x + 1, y + 1, WHITE);
            }
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int maze[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 1, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 1, 1, 0, 1, 1},
        {1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 0, 1, 0, 0, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    drawMaze(maze);

    struct Point start = {1, 1};
    struct Point end = {8, 8};

    struct Point parent[ROWS][COLS] = { { { -1, -1 } } };
    int cost[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cost[i][j] = INT_MAX;
        }
    }

    aStar(maze, start, end, parent, cost);
    drawPath(parent, start, end);

    getch();
    closegraph();
    return 0;
}

