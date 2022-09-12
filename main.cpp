#include <iostream>
#include <string>
#include <queue>
#include <vector>


void
getVector(int col, int line, std::vector<std::vector<char>> &matrix, std::queue<std::pair<int, int>> &queue_matrix);

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int col = 0, line = 0;
    std::cin >> col;
    std::cin >> line;
    std::vector<std::vector<char>> matrix(col, std::vector<char>(line));


    for (int i = 0; i < col; i++) {
        std::string tmp;
        std::cin >> tmp;
        matrix[i] = std::vector<char>(tmp.begin(), tmp.end());
    }


    std::queue<std::pair<int, int>> queue_matrix;
    getVector(col, line, matrix, queue_matrix);

    while (!queue_matrix.empty()) {
        std::pair first_pair = queue_matrix.front();
        queue_matrix.pop();

        if (matrix[first_pair.first][first_pair.second + 1] == '.') {
            queue_matrix.push(std::pair<int, int>(first_pair.first, first_pair.second + 1));
            matrix[first_pair.first][first_pair.second + 1] = 'L';
        }
        if (matrix[first_pair.first][first_pair.second - 1] == '.') {
            queue_matrix.push(std::pair<int, int>(first_pair.first, first_pair.second - 1));
            matrix[first_pair.first][first_pair.second - 1] = 'R';
        }
        if (matrix[first_pair.first + 1][first_pair.second] == '.') {
            queue_matrix.push(std::pair<int, int>(first_pair.first + 1, first_pair.second));
            matrix[first_pair.first + 1][first_pair.second] = 'U';
        }
        if (matrix[first_pair.first - 1][first_pair.second] == '.') {
            queue_matrix.push(std::pair<int, int>(first_pair.first - 1, first_pair.second));
            matrix[first_pair.first - 1][first_pair.second] = 'D';
        }
    }

    for (int i = 0; i < col; i++) {
        for (int j = 0; j < line; j++) {
            std::cout << matrix[i][j];
        }
        if (i + 1 != col) {
            std::cout << std::endl;
        }
    }
    return 0;
}

void
getVector(int col, int line, std::vector<std::vector<char>> &matrix, std::queue<std::pair<int, int>> &queue_matrix) {
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < line; j++) {
            if (matrix[i][j] == 'S') {
                if (matrix[i][j + 1] == '.') {
                    queue_matrix.push(std::pair<int, int>(i, j + 1));
                    matrix[i][j + 1] = 'L';
                }
                if (matrix[i][j - 1] == '.') {
                    queue_matrix.push(std::pair<int, int>(i, j - 1));
                    matrix[i][j - 1] = 'R';
                }
                if (matrix[i + 1][j] == '.') {
                    queue_matrix.push(std::pair<int, int>(i + 1, j));
                    matrix[i + 1][j] = 'U';
                }
                if (matrix[i - 1][j] == '.') {
                    queue_matrix.push(std::pair<int, int>(i - 1, j));
                    matrix[i - 1][j] = 'D';
                }
                return;
            }
        }
    }
    return;
}
