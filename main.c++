#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <time.h>
#include <conio.h>
#include <cctype>

using namespace std;

#define clear() printf("\e[1;1H\e[2J");

#define N 9
#define TOTAL N * N
#define SIZE 1000

#define BLUE_FG "\033[38;5;39m"
#define DARK_BLUE_FG "\033[38;5;234m"
#define RED_FG "\033[38;5;197m"
#define WHITE_FG "\033[38;5;15m"
#define RESET "\033[m"

class Grid {
    public:
        char sudoku_grid[N][N];
        char solution_grid[N][N];
        char original_grid[N][N];
        int current_row, current_col, errors;
};

void initializeGrid(Grid *myGrid) {
    srand(time(NULL));
    fstream fptr;
    fptr.open("sudoku.csv", ios::in);

    string line;
    int random_pos = rand() % 100;
    int count = 0;
    while (getline(fptr, line)) {
        if (random_pos == count) {
            fptr.close();
            break;
        }
        count++;
    }
    count = 0;
    // cout << line << endl;
    // _sleep(3000);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            myGrid->sudoku_grid[i][j] = line[count]; myGrid->original_grid[i][j] = line[count];
            myGrid->solution_grid[i][j] = line[((TOTAL) + 1) + count];
            count++;
        }
    }
}

void printGrid(Grid *myGrid) {
    clear();
    for (int row = 0; row < N; row++) {
        if (row % 3 == 0 && row != 0) {
                cout << "━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━" << endl;
        }
        for (int col = 0; col < N; col++) {
            if (col % 3 == 0 && col != 0) {
                cout << "┃  ";
            }
            if (row == myGrid->current_row && col == myGrid->current_col){
                cout << WHITE_FG << myGrid->sudoku_grid[row][col] << RESET << "  ";
            } else if (myGrid->original_grid[row][col] == '0') {
                if (myGrid->solution_grid[row][col] == myGrid->sudoku_grid[row][col]) {
                    cout << BLUE_FG << myGrid->sudoku_grid[row][col] << RESET << "  ";
                } else if (myGrid->solution_grid[row][col] != myGrid->sudoku_grid[row][col] && myGrid->sudoku_grid[row][col] != '0') {
                    cout << RED_FG << myGrid->sudoku_grid[row][col] << RESET << "  ";
                } else {
                    cout << "." << "  ";
                }
            } else {
                cout << DARK_BLUE_FG << myGrid->sudoku_grid[row][col] << RESET << "  ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void navigateGrid(Grid *myGrid) {
    char c;
    int row = myGrid->current_row;
    int col = myGrid->current_col;
    _kbhit();
    switch(c = tolower(getch())) {
        case 'w':
            if (row != 0) {myGrid->current_row -= 1;}
            break;
        case 'a':
            if (col != 0) {myGrid->current_col -= 1;}
            break;
        case 's':
            if (row != (N - 1)) {myGrid->current_row += 1;}
            break;
        case 'd':
            if (col != (N - 1)) {myGrid->current_col += 1;}
            break;
        default:
            break;     
    }
    for (int i = 0; i < N + 1; i++) {
        if (i == c - '0' && myGrid->original_grid[row][col] == '0') {
            myGrid->sudoku_grid[row][col] = c;
        }
    }
}

bool endGame(Grid *myGrid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (myGrid->solution_grid[i][j] != myGrid->sudoku_grid[i][j]) {
                return true;
            }
        }
    }
    myGrid->errors += 1;
    return false;
}

int main() {
    Grid myGrid;
    myGrid.current_row = myGrid.current_col = N / 2;
    myGrid.errors = 0;
    initializeGrid(&myGrid);
    do {
        printGrid(&myGrid);
        navigateGrid(&myGrid);
    } while (endGame(&myGrid) || myGrid.errors == 3);
    return 0;
}
