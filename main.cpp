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
typedef std::map<int, bool> used;
typedef std::map<std::string, bool> build_list;
typedef std::vector<std::vector<int>> adjacency;
typedef std::map<int, std::map<int, int>> adjacency2;


std::string generateKey(int a, int b) {
    return std::to_string(a) + "-" + std::to_string(b);
}

void printc(connection con) {
    std::cout<<con.first.first << " " << con.first.second << " " << con.second << "\n";
}

void print(list *l){
    for (auto con: *l) {
        printc(con);
    }
}

//bool compare(connection a, connection b, build_list *build) {
//    if ((*build)[generateKey(b)]) {
//        return false;
//    }
//
//    if ((*build)[generateKey(a)]) {
//        return true;
//    }
//    return a.second < b.second;
//}

void prepareUse(list *exist, list *miss, adjacency *adj, build_list *build) {
    for (auto con: *miss) {
        (*adj)[con.first.first][con.first.second] = con.second;
        (*adj)[con.first.second][con.first.first] = con.second;
    }

    for (auto con: *exist) {
        (*adj)[con.first.first][con.first.second] = 1;
        (*adj)[con.first.second][con.first.first] = 1;
        (*build)[generateKey(con.first.first, con.first.second)] = true;
    }
}



list prim(list *exist, list *miss, int n) {
    used use;
    adjacency adj;
    list result;
    build_list build;

    adj.resize(n + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i < n + 1; i++) {
        adj.push_back(std::vector<int>(n + 1, 0));
    }

    prepareUse(exist, miss, &adj, &build);
    use[1] = true;
    int no_edge = 0;

    while (no_edge < n - 1 + exist->size() ) {
        int min = INT_MAX;
        int x = 0;
        int y = 0;

        for (auto used_node: use) {
            int i = used_node.first;
            for (int j = 1; j < n + 1; j++) {
                if ( adj[i][j]) { // not in selected and there is an edge
                    if (!use[j]) {
                        use.erase(j);
                        if (min > adj[i][j] || build[generateKey(i, j)] || build[generateKey(j, i)]) {
                            min = adj[i][j];
                            x = i;
                            y = j;
                        }
                    }
                }
            }
        }
        if (x == 0 && y == 0) {
            break;
        }

        if (!build[generateKey(x, y)] && !build[generateKey(y, x)]) {
            std::cout << x <<  " - " << y << " :  " << adj[x][y];
            std::cout << std::endl;
        } else {
            std::cout << "- " << x <<  " - " << y << " :  " << adj[x][y];
            std::cout << std::endl;
        }
        use[y] = true;
        no_edge++;
    }

    return result;
}

void prepareUse2(list *exist, list *miss, adjacency2 *adj, build_list *build) {
    for (auto con: *miss) {
        (*adj)[con.first.first][con.first.second] = con.second;
        (*adj)[con.first.second][con.first.first] = con.second;
    }

    for (auto con: *exist) {
        (*adj)[con.first.first][con.first.second] = -1;
        (*adj)[con.first.second][con.first.first] = -1;
        (*build)[generateKey(con.first.first, con.first.second)] = true;
    }
}

list prim2(list *exist, list *miss, int n) {
    used use;
    adjacency2 adj;
    list result;
    build_list build;

    prepareUse2(exist, miss, &adj, &build);
    use[1] = true;
    int no_edge = 0;

    while (no_edge < n - 1 + exist->size() ) {
        int min = INT_MAX;
        int x = 0;
        int y = 0;

        for (auto used_node: use) {
            if (!used_node.second) {
                continue;
            }
            int i = used_node.first;
            for (auto node: adj[i]) {
                int j = node.first;
                if (!use[j]) {
                    int w = adj[i][j];
                    if (min > w) {
                        min = w;
                        x = i;
                        y = j;
                        if (w < 0) {
                            break;
                        }
                    }
                }
            }
        }
        if (x == 0 && y == 0) {
            int a = 1;
            break;
        }

        int w = adj[x][y];
        if (w > 0) {
            connection con;
            con.first.first = x;
            con.first.second = y;
            con.second = w;
            result.push_back(con);
            adj[x].erase(y);
        } else {
            if (no_edge % 25 == 0) {
                std::cout<<no_edge << "\n";
            }
            std::cout << "- " << x <<  " - " << y << " :  " << adj[x][y];
            std::cout << std::endl;
        }
        use[y] = true;
        no_edge++;
    }

    return result;
}


int main(int argc, char **argv) {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    std::map<int, int> map;

    int n, ex;
    in >> n >> ex;

    list exist_cons;
    for (int i = 0; i < ex; i++){
        connection conn;
        in >> conn.first.first >> conn.first.second;
        conn.second = 0;
        exist_cons.push_back(conn);
    }
    list cons;
    while (in.peek() != EOF && !in.eof()) {
        connection conn;
        in >> conn.first.first >> conn.first.second >> conn.second;
        if (conn.first.first == 0) {
            break;
        }
        cons.push_back(conn);
    }

    list min_cons;
    int sum = 0;
    min_cons = prim2(&exist_cons, &cons, n);
    out << "🛣️🚗Дороги: \n";
    for (auto con: min_cons) {
        sum += con.second;
        if (con.second > 0) {
            out<<con.first.first << "-" << con.first.second << " " << con.second << "\n";
        }
    }
    out << "\n";
    out << "👍Сумма: " << sum;
}
