/*
24. На  плоскости  расположена  система  из  N  шестеренок,
которая  приводится  в  движение  вращением  шестеренки  1  по
часовой стрелке.  Сцепленные шестеренки могут вращаться только
в    противоположных    направлениях.   Требуется   определить
направление движения каждой шестеренки  либо  установить,  что
систему   заклинит.   Некоторые   шестеренки   могут  остаться
неподвижными (11).
Выполнил: Веселов Максим ПС-21
IDE: CLion
C++ 17
*/

#include<windows.h>
#include<string>
#include<iostream>
#include<vector>
#include <fstream>

#define PAIR_START '('
#define PAIR_END ')'
#define ID_SEPARATOR ','

#define ANSWER_ERROR_GEAR  "Шестеренки заклинит"
#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"

#define LEFT "left"
#define RIGHT "right"
#define EMPTY "no"

const int MAX_SIZE = 10000;

enum Direction {
    left = 1,
    right = 0,
    empty = -1,
};

struct QueueNode {
    int data;
    QueueNode* next;
};

// Структура для очереди
struct Queue {
    QueueNode* front;
};

// Функция для создания нового элемента очереди
QueueNode* createQueueNode(int data) {
    QueueNode* newNode = new QueueNode;
    newNode->data = data;
    newNode->next = nullptr;
    return newNode;
}

// Функция для инициализации очереди
Queue* createQueue() {
    Queue* queue = new Queue;
    queue->front = nullptr;
    return queue;
}

// Функция для добавления элемента в конец очереди
void push(Queue* queue, int data) {
    QueueNode* newNode = createQueueNode(data);
    if (queue->front == nullptr) {
        queue->front = newNode;
        return;
    }
    newNode->next = queue->front;
    queue->front = newNode;
}

bool emptyq(Queue* queue) {
    return queue->front == nullptr;
}

int pop(Queue* queue) {
    if (emptyq(queue)) {
        return -1;
    }

    int data = queue->front->data;
    QueueNode* temp = queue->front;

    queue->front = queue->front->next;

    delete temp;

    return data;
}

struct Pair{
    int a;
    int b;
};

Pair convertStringToPairGear(const std::string& str_pair) {
    Pair pair{};
    bool is_one = true;
    for (char ch : str_pair) {
        if (ch == PAIR_START) continue;
        if (ch == PAIR_END) break;
        if (ch == ID_SEPARATOR) {
            is_one = false;
            continue;
        }
        if (is_one) {
            pair.a = pair.a * 10  + (ch - '0');
        } else {
            pair.b = pair.b * 10  + (ch - '0');
        }
    }
    return pair;
}

void writeGearInfo(std::ofstream *out_f, int id, Direction dir) {
    *out_f<< id + 1 << ':' <<((dir == left) ? LEFT : (dir == right) ? RIGHT : (dir == empty) ? EMPTY: "") << "\n";
}


int main(int argc, char *argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<std::vector<int>> connections(MAX_SIZE, std::vector<int>(MAX_SIZE, 0));
    std::vector<int> rotate(MAX_SIZE, -2);
    Queue* q = createQueue();

    std::ifstream in_f;
    in_f.open(INPUT_FILE_NAME);

    std::ofstream out_f;
    out_f.open(OUTPUT_FILE_NAME);
    while (!in_f.eof()) {
        char ch;
        std::string s;
        while (ch != '\n' && !in_f.eof()) {
            in_f.get(ch);
            s += ch;
        }
        if (!in_f.eof()) {
            in_f.get(ch);
        }
        Pair pair = convertStringToPairGear(s);
        connections[pair.a - 1][pair.b - 1] = 1;
        connections[pair.b - 1][pair.a - 1] = 1;
        rotate[pair.a - 1] = -1;
        rotate[pair.b - 1] = -1;
    }

    rotate[0] = 0;
    push(q, 0);

    bool rotationPossible = true;

    while (!emptyq(q) && rotationPossible) {
        int currentGear = pop(q);

        for (int i = 0; i < MAX_SIZE; i++) {
            if (rotate[i] == -2) {
                continue;
            }
            if (connections[currentGear][i] == 1) {
                if (rotate[i] == -1) {
                    rotate[i] = 1 - rotate[currentGear];
                    push(q, i);
                } else if (rotate[i] == rotate[currentGear]) {
                    rotationPossible = false;
                    break;
                }
            }
        }
    }

    if (!rotationPossible) {
        std::cout<<ANSWER_ERROR_GEAR<<"\n";
        return 0;
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        if (rotate[i] == -2) {
            continue;
        }
        writeGearInfo(&out_f, i, Direction(rotate[i]));
    }
    in_f.close();
    out_f.close();
    return 0;
}
