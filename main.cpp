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
typedef std::map<std::string, bool> builded;
typedef std::map<int, std::vector<connection>> adjacency;
typedef std::map<int, bool> nodes;
typedef std::vector<int> list_index;


std::string generateKey(connection con) {
    return std::to_string(con.first.first) + "-" + std::to_string(con.first.second);
}

void printc(connection con) {
    std::cout<<con.first.first << " " << con.first.second << " " << con.second << "\n";
}

void print(list *l){
    for (auto con: *l) {
        printc(con);
    }
}

bool compare(connection a, connection b, builded *build) {
    if ((*build)[generateKey(b)]) {
        return false;
    }

    if ((*build)[generateKey(a)]) {
        return true;
    }
    return a.second < b.second;
}

bool equal(connection a, connection b) {
    return a.first.first == b.first.first && a.first.second == b.first.second;
}

void prepareUse(list *exist, list *miss, adjacency *adj, builded *build) {
    for (auto con: *miss) {
        (*adj)[con.first.first].push_back(con);
        (*adj)[con.first.second].push_back(con);
    }

    for (auto con: *exist) {
        (*adj)[con.first.first].push_back(con);
        (*adj)[con.first.second].push_back(con);
        (*build)[generateKey(con)] = true;
    }
}



list prim(list *exist, list *miss) {
    used use;
    adjacency adj;
    list result;
    builded build;
    nodes list_nodes;

    prepareUse(exist, miss, &adj, &build);

    connection min_con;
    min_con.second = INT_MAX;
    for (auto con: *exist) {
        if (con.second < min_con.second && !build[generateKey(con)]) {
            min_con = con;
        }
    }
    for (auto con: *miss) {
        if (con.second < min_con.second && !build[generateKey(con)]) {
            min_con = con;
        }
    }
    printc(min_con);
    if (min_con.second == INT_MAX) {
        return result;
    }
    use[min_con.first.first] = true;
    list_nodes[min_con.first.first];

    while (true) {
        connection min_con;
        min_con.second = INT_MAX;
        int min_node;
        for (auto one_node : list_nodes) {
            auto node = one_node.first;
            list_index on_delete;
            list cons = adj[node];
            for (int i = 0; i < cons.size(); i++) {
                auto con = cons[i];
                if (use[con.first.first] && use[con.first.second] && !build[generateKey(con)]) {
                    on_delete.push_back(i);
                    continue;
                }
                if (!use[con.first.first] && !use[con.first.second]) {
                    continue;
                }
                if (compare(con, min_con, &build)) {
                    min_con = con;
                    min_node = node;
                }
            }

            for( auto delete_node: on_delete) {
                cons.erase(cons.begin() + delete_node);
            }
        }

        if (min_con.second == INT_MAX) {
            break;
        }

        printc(min_con);
        use[min_con.first.first] = true;
        use[min_con.first.second] = true;

        list_nodes.erase(min_node);
        list_nodes[min_con.first.first];
        list_nodes[min_con.first.second];


        auto cons = adj[min_con.first.first];
        int index = -1;
        for (int i = 0;i < cons.size(); i ++) {
            connection con = cons[i];
            if (equal(con, min_con)) {
                index = i;
                break;
            }
        }
        cons.erase(cons.begin() + index);
        adj[min_con.first.first] = cons;

        cons = adj[min_con.first.second];
        index = -1;
        for (int i = 0; i  < cons.size(); i ++) {
            connection con = cons[i];
            if (equal(con, min_con)) {
                index = i;
                break;
            }
        }
        cons.erase(cons.begin() + index);
        adj[min_con.first.second] = cons;

        result.push_back(min_con);
    }
    return result;
}

int main(int argc, char **argv) {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    std::map<int, int> map;

    int bin, ex;
    in >> bin >> ex;

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
    min_cons = prim(&exist_cons, &cons);
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
