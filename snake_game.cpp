#include <iostream>
#include <ncurses.h>
#include <unistd.h>

bool gameOver;
const int width = 20;
const int height = 10;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    initscr(); // Initialize ncurses
    clear();
    noecho();  // Don't echo user input
    cbreak();  // Line buffering disabled
    curs_set(0);  // Hide the cursor
    timeout(100); // Set a delay for getch()

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    clear();

    for (int i = 0; i < width + 2; i++)
        mvaddch(0, i, '#');

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || j == width - 1)
                mvaddch(i + 1, j, '#');

            if (i == y && j == x)
                mvaddch(i + 1, j, 'O');
            else if (i == fruitY && j == fruitX)
                mvaddch(i + 1, j, 'F');
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        mvaddch(i + 1, j, 'o');
                        print = true;
                    }
                }
                if (!print)
                    mvaddch(i + 1, j, ' ');
            }
        }
    }

    for (int i = 0; i < width + 2; i++)
        mvaddch(height + 1, i, '#');

    mvprintw(height + 2, 0, "Score: %d", score);
    refresh();
}

void Input() {
    int key = getch();
    switch (key) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000); // Sleep for 100ms
    }
    endwin(); // Cleanup ncurses
    return 0;

}
