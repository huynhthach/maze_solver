#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <graphics.h>

// K�ch thu?c m� cung
#define ROWS 10
#define COLS 10

// K�ch thu?c � vu�ng
#define SQUARE_SIZE 40

// C?u tr�c d? luu t?a d? c?a m?t � trong m� cung
struct Point {
    int x, y;
};

// H�m ki?m tra xem � c� n?m trong m� cung v� kh�ng ph?i l� tu?ng hay d� du?c duy?t chua
bool isValid(int maze[ROWS][COLS], int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0);
}
// H�m t�nh kho?ng c�ch Manhattan gi?a hai di?m
int manhattanDistance(struct Point p1, struct Point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

// H�m th?c hi?n thu?t to�n A*
void aStar(int maze[ROWS][COLS], struct Point start, struct Point end, struct Point parent[ROWS][COLS], int cost[ROWS][COLS]) {
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    // H�ng d?i uu ti�n d�ng d? luu c�c � chua duy?t theo th? t? uu ti�n c?a A*
    struct Point openList[ROWS * COLS];
    int openListSize = 0;

    // Danh s�ch luu l?i c�c � d� duy?t
    bool closedList[ROWS][COLS] = {false};

    // Kh?i t?o gi� tr? cost c?a di?m b?t d?u l� 0
    cost[start.x][start.y] = 0;

    // Th�m di?m b?t d?u v�o h�ng d?i uu ti�n
    openList[openListSize++] = start;

    while (openListSize > 0) {
        // T�m � c� u?c lu?ng chi ph� nh? nh?t trong h�ng d?i uu ti�n
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

        // Di chuy?n � c� u?c lu?ng chi ph� nh? nh?t ra kh?i h�ng d?i uu ti�n
        for (int i = minIndex; i < openListSize - 1; i++) {
            openList[i] = openList[i + 1];
        }
        openListSize--;

        // ��nh d?u � hi?n t?i l� d� duy?t
        closedList[current.x][current.y] = true;

        // N?u t�m th?y � d�ch, tho�t kh?i v�ng l?p
        if (current.x == end.x && current.y == end.y) {
            break;
        }

        // Duy?t qua c�c � l�n c?n
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
// H�m v? l? tr�nh t�m du?c t? A*
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

