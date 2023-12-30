#include<iostream>
#include "raylib.h"

Color backgroundColor = { 55, 53, 50 };

const int screenWidth = 600;
const int screenHeight = 600;
const int cellSize = 200;
int board[3][3] = { 0 };  // 0 for empty, 1 for X, 2 for O
int currentPlayer = 1;
int winner = 0;
bool gameOver = false;
bool restartButtonPressed = false;

void drawBoard() {
    // Draw the Tic Tac Toe grid
    for (int i = 1; i < 3; i++) {
        // Horizontal lines
        DrawRectangle(25, i * cellSize - 5, 525, 5, WHITE);

        // Vertical lines
        DrawRectangle(i * cellSize - 5, 25, 5, 550, WHITE);
    }

}

void drawXO() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == 1) {
                DrawText("X", col * cellSize + cellSize / 2 - 20, row * cellSize + cellSize / 2 - 20, 80, DARKGRAY);
            }
            else if (board[row][col] == 2) {
                DrawText("O", col * cellSize + cellSize / 2 - 20, row * cellSize + cellSize / 2 - 20, 80, DARKGRAY);
            }
        }
    }

}

void resetGame() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            board[row][col] = 0;
        }
    }
    currentPlayer = 1;
    winner = 0;
    gameOver = false;
    restartButtonPressed = false;
}

int evaluate(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 2)
                return 10;
            else if (board[i][0] == 1)
                return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 2)
                return 10;
            else if (board[0][i] == 1)
                return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 2)
            return 10;
        else if (board[0][0] == 1)
            return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 2)
            return 10;
        else if (board[0][2] == 1)
            return -10;
    }

    return 0;
}

bool isMovesLeft(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0)
                return true;
        }
    }
    return false;
}

int minimax(int board[3][3], bool isMax) {
    int score = evaluate(board);
    if (score == 10) {
        return score;
    }
    if (score == -10) {
        return score;
    }
    if (!isMovesLeft(board)) {
        return 0;
    }
    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 2;
                    best = fmax(best, minimax(board, !isMax));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
    else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = 1;
                    best = fmin(best, minimax(board, !isMax));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

void findBestMove(int board[3][3]) {
    int bestVal = -1000;
    int bestMove[2] = { -1, -1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 0) {
                board[i][j] = 2;
                int moveVal = minimax(board, false);
                board[i][j] = 0;

                if (moveVal > bestVal) {
                    bestMove[0] = i;
                    bestMove[1] = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    if (bestMove[0] != -1 && bestMove[1] != -1) {
        board[bestMove[0]][bestMove[1]] = 2;
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) resetGame();
        BeginDrawing();
        ClearBackground(backgroundColor);
        drawBoard();
        drawXO();
        if (!gameOver) {
            if (currentPlayer == 1) {
                int mouseX = GetMouseX();
                int mouseY = GetMouseY();
                int row = mouseY / cellSize;
                int col = mouseX / cellSize;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && board[row][col] == 0) {
                    board[row][col] = currentPlayer;
                    currentPlayer = 2;
                }
            }
            else {
                findBestMove(board);
                currentPlayer = 1;
            }
            bool draw = true;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == 0) {
                        draw = false;
                        break;
                    }
                }
            }
            if (draw) {
                gameOver = true;
                winner = 0;
            }
            for (int i = 0; i < 3; i++) {
                if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 0) {
                    winner = board[i][0];
                    gameOver = true;
                }
                if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != 0) {
                    winner = board[0][i];
                    gameOver = true;
                }
            }
            if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 0) {
                winner = board[0][0];
                gameOver = true;
            }
            if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 0) {
                winner = board[0][2];
                gameOver = true;
            }
        }
        if (gameOver) {
            if (winner == 0) {
                DrawText("It's a draw!", 200, 260, 40, RED);
            }
            else {
                DrawText(winner == 1 ? "Player X wins!" : "Player O wins!", 160, 260, 40, RED);
            }
        }
        DrawText("Press 'R' to Restart", 20, 0, 10, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}