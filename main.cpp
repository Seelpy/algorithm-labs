/*
28. На равнинной местности имеется сеть автомобильных дорог с
двусторонним движением. Для каждой пары городов, не  связанных
дорогой,  известно расстояние по прямой между ними. Оказалось,
что некоторые  пары городов не имеют автомобильного сообщения.
Принято  решение  построить новые дороги, связывающие напрямую
некоторые  города, так,  чтобы существовал путь  между  любыми
двумя  городами, а общая  длина новых  дорог была  минимальна.
Найти такие  дороги и определить их общую длину (12).
IDE: CLion
C++ 17
*/

#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <map>

typedef std::pair<int, int> pair;
typedef std::pair<pair, int> connection;
typedef std::vector<connection> list;
typedef std::map<int, bool> used;

void prepareUse(list *exist, list *miss, list *result, list *q, used *use) {
    for (auto con: *miss) {
        (*use)[con.first.first] = false;
        (*use)[con.first.second] = false;
        (*q).push_back(con);
    }

    for (auto con: *exist) {
        (*use)[con.first.first] = true;
        (*use)[con.first.second] = true;
    }
}



list prim(list *exist, list *miss) {
    used use;
    list q;
    list result;

    prepareUse(exist, miss, &result, &q, &use);

    if (q.empty()) {
        return result;
    }

    connection con = q[0];

    if (exist->empty()) {
        use[con.first.first] = true;
        use[con.first.second] = true;
        result.push_back(con);
    }

    for (int i = 0; i < use.size() - 1; i++) {
        connection min_con;
        min_con.second = INT_MAX;
        for (auto con: q) {
            if ((use[con.first.first] || use[con.first.second]) &&
                !(use[con.first.first] && use[con.first.second]) &&
                    con.second < min_con.second) {
                min_con = con;
            }
        }
        if (min_con.second == INT_MAX) {
            break;
        }
        result.push_back(min_con);
        use[min_con.first.first] = true;
        use[min_con.first.second] = true;
    }
    return result;
}

int main(int argc, char **argv) {
    std::ifstream ext("exist.txt");
    std::ifstream nw("new.txt");
    std::ofstream out("output.txt");

    std::map<int, int> map;

    list exist_cons;
    while (ext.peek() != EOF && !ext.eof()) {
        connection conn;
        ext >> conn.first.first >> conn.first.second >> conn.second;
        exist_cons.push_back(conn);
    }
    list cons;
    while (nw.peek() != EOF && !nw.eof()) {
        connection conn;
        nw >> conn.first.first >> conn.first.second >> conn.second;
        cons.push_back(conn);
    }

    list min_cons;
    int sum = 0;
    min_cons = prim(&exist_cons, &cons);
    out << "🛣️🚗Ways: \n";
    for (auto con: min_cons) {
        sum += con.second;
        out << con.first.first << " - " << con.first.second << " | " << con.second << "\n";
    }
    out << "\n";
    out << "👍Sum: " << sum;
}
