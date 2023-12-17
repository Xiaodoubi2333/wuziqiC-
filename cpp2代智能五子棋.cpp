#include <iostream>
#include <climits>
using namespace std;

// �������̴�С
const int BOARD_SIZE = 15;

// ��������
char board[BOARD_SIZE][BOARD_SIZE];

// ��ʼ������
void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}

// ��ӡ����
void printBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// �ж���Ϸ�Ƿ����
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

// ���λ���Ƿ�Ϸ�
bool isValidMove(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }

    if (board[x][y] != '-') {
        return false;
    }

    return true;
}

// �û�����
void userMove(char playerSymbol) {
    int x, y;
    do {
        cout << "��� " << playerSymbol << " ��������ƶ� (x y): ";
        cin >> x >> y;
    } while (!isValidMove(x, y));

    board[x][y] = playerSymbol;
}

// AI��������
int evaluate(int x, int y, char playerSymbol) {
    int score = 0;

    // ������
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

    // �������
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

    // ������Խ���
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

    // ��鸱�Խ���
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

// Alpha-Beta��֦
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
    cout << "��ѡ����Ϸģʽ��\n";
    cout << "1. ˫�˶�ս\n";
    cout << "2. ���˶�ս��AI��\n";
    cin >> mode;

    initBoard();
    char player1Symbol = 'X';
    char player2Symbol = 'O';

    while (!isGameOver()) {
        cout << "================= ���� ===============" << endl;
        printBoard();

        if (mode == 1) {
            // ˫�˶�ս
            userMove(player1Symbol);
        } else if (mode == 2) {
            // ���˶�ս��AI��
            if (player1Symbol == 'X') {
                userMove(player1Symbol);
            } else {
                aiMove(player2Symbol);
            }
        }

        // �л����
        swap(player1Symbol, player2Symbol);
    }

    printBoard();
    cout << "��Ϸ����!" << endl;

    return 0;
}
