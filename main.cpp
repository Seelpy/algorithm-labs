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
#include <string>

typedef std::pair<int, int> pair;
typedef std::pair<pair, int> connection;
typedef std::vector<connection> list;

void addEdge(std::vector<pair> adj[], connection con) {
    adj[con.first.first].emplace_back(con.first.second, con.second);
    adj[con.first.second].emplace_back(con.first.first, con.second);
}

list prim(std::vector<pair> adj[], int n) {
    std::priority_queue<pair, std::vector<pair>, std::greater<pair> > pq;

    std::vector<int> key(n, INT_MAX);
    std::vector<int> parent(n, -2);
    std::vector<bool> useage(n, false);
    int src = 0;
    pq.emplace(0, src);

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (useage[u]) {
            continue;
        }

        useage[u] = true;
        for (auto x: adj[u]) {
            int v = x.first;
            int weight = x.second;
            if (!useage[v] && key[v] > weight) {
                key[v] = weight;
                pq.emplace(key[v], v);
                parent[v] = u;
            }
        }
    }

    list result;

    for (int i = 1; i < n; ++i) {
        connection con;
        con.first.first = parent[i] + 1;
        con.first.second = i + 1;
        con.second = key[i];
        if (con.second  > -1) {
            result.push_back(con);
        }
    }


    return result;
}


int main(int argc, char **argv) {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    std::map<int, int> map;

    int n, ex;
    in >> n >> ex;

    std::vector<pair> adj[n];

    list exist_cons;
    for (int i = 0; i < ex; i++) {
        connection con;
        in >> con.first.first >> con.first.second;
        con.first.first--;
        con.first.second--;
        con.second = -1;
        addEdge(adj, con);
    }
    list cons;
    while (in.peek() != EOF && !in.eof()) {
        connection con;
        in >> con.first.first >> con.first.second >> con.second;
        con.first.first--;
        con.first.second--;
        if (con.first.first < 0) {
            break;
        }
        addEdge(adj, con);
    }

    list min_cons;
    int sum = 0;
    min_cons = prim(adj, n);
    out << "🛣️🚗Дороги: \n";
    for (auto con: min_cons) {
        sum += con.second;
        if (con.second > 0) {
            out << con.first.first << "-" << con.first.second << " " << con.second << "\n";
        }
    }
    out << "\n";
    out << "👍Сумма: " << sum;
}
