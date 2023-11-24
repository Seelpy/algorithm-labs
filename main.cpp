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

struct Checker {
private:
    used *use;
public:
    explicit Checker(used *use) {
        this->use = use;
    }
    bool bigger(connection const &x, connection const &y) {
        bool cx = (*use)[x.first.first] || (*use)[x.first.second];
        bool cy = (*use)[y.first.first] || (*use)[y.first.second];
        if (cx && cy || !(cx || cy)) {
            return x.second > y.second;
        }
        if (cy) {
            return true;
        }
        return false;
    }
};

struct Connection {
private:
    Checker *checker;
public :
    connection con;

    Connection(Checker *checker, connection con) {
        this->checker = checker;
        this->con = con;
    }

    friend bool operator<(Connection const &x, Connection const &y) {
        return x.checker->bigger(x.con, y.con);
    }
};

typedef std::priority_queue<Connection> queue;

void prepareUse(list *exist, list *miss, list *result, queue *q, used *use) {
    Checker checker(use);
    for (auto con: *miss) {
        (*use)[con.first.first] = false;
        (*use)[con.first.second] = false;
        auto tmp = Connection(&checker, con);
        (*q).push(tmp);
    }

    for (auto con: *exist) {
        (*use)[con.first.first] = true;
        (*use)[con.first.second] = true;
    }
}

list prim(list *exist, list *miss) {
    used use;
    queue q;
    list result;

    prepareUse(exist, miss, &result, &q, &use);

    if (q.empty()) {
        return result;
    }

    Connection con = q.top();

    if (exist->empty()) {
        use[con.con.first.first] = true;
        use[con.con.first.second] = true;
        result.push_back(con.con);
    }

    while (!q.empty()) {
        con = q.top();
        q.pop();
        if (use[con.con.first.first] && use[con.con.first.second]) {
            continue;
        }
        if (!use[con.con.first.first] && !use[con.con.first.second]) {
            q.push(con);
            continue;
        }

        result.push_back(con.con);
        use[con.con.first.first] = use[con.con.first.second] = true;
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
