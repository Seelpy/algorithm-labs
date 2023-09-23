/*
24. Ќа  плоскости  расположена  система  из  N  шестеренок,
котора€  приводитс€  в  движение  вращением  шестеренки  1  по
часовой стрелке.  —цепленные шестеренки могут вращатьс€ только
в    противоположных    направлени€х.   “ребуетс€   определить
направление движени€ каждой шестеренки  либо  установить,  что
систему   заклинит.   Ќекоторые   шестеренки   могут  остатьс€
неподвижными (11).
¬ыполнил: ¬еселов ћаксим ѕ—-21
IDE: CLion
C++ 17
*/

#include<windows.h>
#include<string>
#include<iostream>

#define PAIR_START '('
#define PAIR_END ')'
#define ID_SEPARATOR ','

#define ERROR_FIRST_ID "ѕервый id должен быть равен 1"

#define ANSWER_ERROR_GEAR  "Ўестиренки заклинит"

#define LEFT "left"
#define RIGHT "right"
#define EMPTY "no"

#define LINE_SEPARATOR "\n"

enum Direction {
    left,
    right,
    empty,
};

struct Gear {
    int id ;
    Direction dir;
};

struct PairGear {
    int id_a;
    int id_b;
};

struct GearNode {
    Gear gear;
    GearNode* next;
};

struct GearStore {
    GearNode *head;

    void push(GearNode gear_node) {
        auto* node = new GearNode;
        node->gear = gear_node.gear;
        node->next = gear_node.next;
        if (this->inStore(gear_node.gear.id)) {
            return;
        }
        GearNode *left_node = this->findCorrectPlace(node);
        if (left_node == nullptr) {

            this->head = node;
            return;
        }

        node->next = left_node->next;
        left_node->next = node;
    }

    [[nodiscard]] Gear findGear(int id) const {
        GearNode *node = head;
        while (node != nullptr) {
            if (node->gear.id == id) {
                return node->gear;
            }
            node = node->next;
        }
        return {-1, empty};
    }

    GearNode* findCorrectPlace(GearNode *gear_node) const{
        GearNode *node = this->head;
        if (node == nullptr) {
            return nullptr;
        }

        while (node->next != nullptr && gear_node->gear.id  >= node->gear.id) {
            node = node->next;
        }
        return node;
    }

    [[nodiscard]] bool inStore(int id) const {
        GearNode *node = head;
        while (node != nullptr) {
            if (node->gear.id == id) {
                return true;
            }
            node = node->next;
        }
        return false;
    }

    [[nodiscard]] GearNode* getHead() const{
        return this->head;
    }
};

Direction getCorrectDirection(Direction dir) {
    if (dir == empty) {
        return empty;
    }

    if (dir == left) {
        return right;
    }
    return left;
}

PairGear convertStringToPairGear(const std::string& str_pair) {
    PairGear pair{};
    bool is_one = true;
    for (char ch : str_pair) {
        if (ch == PAIR_START) continue;
        if (ch == PAIR_END) break;
        if (ch == ID_SEPARATOR) {
            is_one = false;
            continue;
        }

        if (is_one) {
            pair.id_a = pair.id_a * 10  + (ch - '0');
        } else {
            pair.id_b = pair.id_b * 10  + (ch - '0');
        }
    }
    return pair;
}

Gear createEptyGear(int id) {
    return {
            id,
            empty
    };
}

bool isNormalDirections(Direction dir_a, Direction dir_b) {
    if (dir_a == empty || dir_b == empty) return true;
    if (dir_a == dir_b) return false;
    return true;
}

void setDirections(Gear *a, Gear *b) {
    if (a->dir == empty) {
        a->dir = getCorrectDirection(b->dir);
    }
    if (b->dir == empty) {
        b->dir = getCorrectDirection(a->dir);
    }
}

void writeGearInfo(Gear g) {
    std::cout<< g.id << ':' <<((g.dir == left) ? LEFT : (g.dir == right) ? RIGHT : (g.dir == empty) ? EMPTY: "");
}

void writeSystemInfo(GearNode *head) {
    GearNode *node = head;

    while (node != nullptr) {
        writeGearInfo(node->gear);
        std::cout<< LINE_SEPARATOR;
        node = node->next;
    }
}


int main(int argc, char *argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bool gear_error = false;
    GearStore store{};
    GearNode init_gear = {{1, right}, nullptr};
    store.push(init_gear);

    for (int i = 1; i < argc; i++) {
        PairGear pair = convertStringToPairGear(std::string(argv[i]));
        int id_a = pair.id_a;
        int id_b = pair.id_b;
        Gear g_a = createEptyGear(id_a);
        Gear g_b = createEptyGear(id_b);

        if (store.inStore(id_a)) {
            Gear tmp_g_a = store.findGear(id_a);
            if (tmp_g_a.id > 0) {
                g_a = tmp_g_a;
            }
        }

        if (store.inStore(id_b)) {
            Gear tmp_g_b = store.findGear(id_b);
            if (tmp_g_b.id > 0) {
                g_b = tmp_g_b;
            }
        }

        if (!(isNormalDirections(g_a.dir, g_b.dir))) {
            std::cout << ANSWER_ERROR_GEAR << LINE_SEPARATOR;
            return 0;
        }

        setDirections(&g_a, &g_b);
        store.push({g_a, nullptr});
        store.push({g_b, nullptr});
    }

    writeSystemInfo(store.getHead());
    return 0;
}
