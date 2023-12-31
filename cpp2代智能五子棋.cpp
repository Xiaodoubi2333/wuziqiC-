#include <iostream>
#include <climits>
using namespace std;

// 定义棋盘大小
const int BOARD_SIZE = 15;

// 定义棋盘
char board[BOARD_SIZE][BOARD_SIZE];

// 初始化棋盘
void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}

// 打印棋盘
void printBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// 判断游戏是否结束
bool isGameOver() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != '-') {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;

                        int count = 0;
                        for (int step = 0; step < 5; step++) {
                            int x = i + step * dx;
                            int y = j + step * dy;

                            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) break;
                            if (board[x][y] != board[i][j]) break;

                            count++;
                        }

                        if (count == 5) return true;
                    }
                }
            }
        }
    }

    return false;
}

// 检查位置是否合法
bool isValidMove(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }

    if (board[x][y] != '-') {
        return false;
    }

    return true;
}

// 用户下棋
void userMove(char playerSymbol) {
    int x, y;
    do {
        cout << "玩家 " << playerSymbol << " 输入你的移动 (x y): ";
        cin >> x >> y;
    } while (!isValidMove(x, y));

    board[x][y] = playerSymbol;
}

// AI评估函数
int evaluate(int x, int y, char playerSymbol) {
    int score = 0;

    // 检查横向
    for (int i = max(0, x - 4); i <= min(BOARD_SIZE - 1, x + 4); i++) {
        int count = 0;
        for (int j = 0; j < 5; j++) {
            if (board[i + j][y] == playerSymbol) {
                count++;
            } else if (board[i + j][y] != '-') {
                count = 0;
                break;
            }
        }
        score += count * count;
    }

    // 检查纵向
    for (int j = max(0, y - 4); j <= min(BOARD_SIZE - 1, y + 4); j++) {
        int count = 0;
        for (int i = 0; i < 5; i++) {
            if (board[x][j + i] == playerSymbol) {
                count++;
            } else if (board[x][j + i] != '-') {
                count = 0;
                break;
            }
        }
        score += count * count;
    }

    // 检查主对角线
    for (int i = max(0, x - 4), j = max(0, y - 4); i <= min(BOARD_SIZE - 1, x + 4) && j <= min(BOARD_SIZE - 1, y + 4); i++, j++) {
        int count = 0;
        for (int k = 0; k < 5; k++) {
            if (board[i + k][j + k] == playerSymbol) {
                count++;
            } else if (board[i + k][j + k] != '-') {
                count = 0;
                break;
            }
        }
        score += count * count;
    }

    // 检查副对角线
    for (int i = max(0, x - 4), j = min(BOARD_SIZE - 1, y + 4); i <= min(BOARD_SIZE - 1, x + 4) && j >= max(0, y - 4); i++, j--) {
        int count = 0;
        for (int k = 0; k < 5; k++) {
            if (board[i + k][j - k] == playerSymbol) {
                count++;
            } else if (board[i + k][j - k] != '-') {
                count = 0;
                break;
            }
        }
        score += count * count;
    }

    return score;
}

// Alpha-Beta剪枝
int alphabeta(int depth, int alpha, int beta, bool isMax, char playerSymbol) {
    if (depth == 0 || isGameOver()) {
        return evaluate(0, 0, playerSymbol);
    }

    if (isMax) {
        int bestValue = INT_MIN;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == '-') {
                    board[i][j] = playerSymbol;
                    int value = alphabeta(depth - 1, alpha, beta, false, playerSymbol);
                    bestValue = max(bestValue, value);
                    board[i][j] = '-';
                    alpha = max(alpha, bestValue);

                    if (beta <= alpha) {
                        return bestValue;
                    }
                }
            }
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == '-') {
                    board[i][j] = (playerSymbol == 'X') ? 'O' : 'X';
                    int value = alphabeta(depth - 1, alpha, beta, true, playerSymbol);
                    bestValue = min(bestValue, value);
                    board[i][j] = '-';
                    beta = min(beta, bestValue);

                    if (beta <= alpha) {
                        return bestValue;
                    }
                }
            }
        }
        return bestValue;
    }
}

int main() {
    int mode;
    cout << "请选择游戏模式：\n";
    cout << "1. 双人对战\n";
    cout << "2. 单人对战（AI）\n";
    cin >> mode;

    initBoard();
    char player1Symbol = 'X';
    char player2Symbol = 'O';

    while (!isGameOver()) {
        cout << "================= 下棋 ===============" << endl;
        printBoard();

        if (mode == 1) {
            // 双人对战
            userMove(player1Symbol);
        } else if (mode == 2) {
            // 单人对战（AI）
            if (player1Symbol == 'X') {
                userMove(player1Symbol);
            } else {
                aiMove(player2Symbol);
            }
        }

        // 切换玩家
        swap(player1Symbol, player2Symbol);
    }

    printBoard();
    cout << "游戏结束!" << endl;

    return 0;
}
