#include <iostream>
#include <string>
#include <set>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string A, B;

    std::cin >> A;
    std::cin >> B;
    std::string cont = B;
    std::unordered_map<char, int> umap;
    for (auto ch: A) {
        auto iter = umap.find(ch);
        if (iter != umap.end()) {
            iter->second += 1;
        } else {
            umap.insert(std::pair<char, int>(ch, 1));
        }

    }
    for (int i = 0; i < A.length(); i++) {
        auto iter = umap.find(B[i]);
        if (A[i] == B[i]) {
            cont[i] = 'P';
            umap[B[i]] -= 1;

        }
    }
    for (int i = 0; i < A.length(); i++) {
        auto iter = umap.find(B[i]);
        if (B[i] != A[i]) {

            if ((iter != umap.end()) && (iter->second >= 1)) {
                cont[i] = 'S';
                umap[B[i]] -= 1;
            } else {
                cont[i] = 'I';
            }
        }
    }
    std::cout << cont;
    return 0;
}
