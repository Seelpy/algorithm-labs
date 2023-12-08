/*
   20. При  заданном  четном  N  (N  <=  16)  перечислить  все
правильные скобочные формы длины N из скобок '(', ')', '[',']'.
Время счета не более 1 сек (11).
Например, для N=4  правильные формы:
(())
([])
()()
()[]
[()]
[[]]
[]()
[][]
IDE: CLion
C++ 17
Веселов Максим ПС-21
*/

#include <vector>
#include <fstream>
#include <map>
#include <bitset>
#include <iostream>
#include <set>
#include "math.h"
#include <stack>

std::vector<int> masks;
std::map<int, std::vector<int>> parentheses;


void generateMasks(int n) {
    for (int i = 0; i < n; i ++ ) {
        masks.push_back(std::pow(2, i));
    }
}

void generateParentheses(int n);

std::vector<int> getParentheses(int n) {
    if (parentheses.find(n) == parentheses.end()) {
        generateParentheses(n);
    }
    return parentheses[n];
}

int buildNum(int a, int b, int dif) {
    int tmp = (a << dif);
    return tmp | b;
}

void generateParentheses(int n) {
    if (n == 0) {
        return;
    }
    if (n ==  2) {
        std::vector<int> t = std::vector<int>{2};
        parentheses[2] = t;
    }
    std::set<int> tmp;
    tmp.insert(buildNum(0, std::pow(2, n/2) - 1, 0));
    for (int dif = 2 ; dif <= n / 2; dif += 2 ) {
        auto left = getParentheses(dif);
        auto right = getParentheses(n - dif);
        for (auto a: left) {
            for (auto b: right) {
                tmp.insert(buildNum(b, a, dif));
                tmp.insert(buildNum(a, b, n - dif));
            }
        }
    }
    parentheses[n] = std::vector(tmp.begin(), tmp.end());
}

std::string generateVersions(int n, int ver, int level, std::string res = "") {
    if (level <= 0) {
        return res;
    }
    auto t = std::bitset<6>(n).to_string();
    if (n & masks[level - 1]) {
        if (ver & 1) {
            return generateVersions(n, ver >> 1, level - 1, res + "]");
        }
        return generateVersions(n, ver >> 1, level - 1, res + ")");

    }
    return generateVersions(n, (ver << 1) + 0, level - 1, res + "(") + "\n" + generateVersions(n, (ver << 1) + 1, level - 1, res +"[");
}

std::string startGenerateVersions(int n, int all) {
    return generateVersions(n, 0, all, "") + "\n";
}

int main(int argc, char** argv) {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int n;
    in >> n;
    generateMasks(n);
    getParentheses(n);
    for (auto l: parentheses[n]) {
        out << (std::bitset<16>(l)>>(16 - n)).to_string() << "\n";
        out << startGenerateVersions(l, n);
    }
}
