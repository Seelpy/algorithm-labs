/*
28. В офисе фирмы  Megasoft  установлены  N  компьютеров  с
номерами от 1 до N, некоторые из них  соединены  между  собой.
Сообщение между соединенными компьютерами проходит в любом  из
двух направлений за 1 с. Компьютер,  получив  сообщение, сразу
отправляет  его  всем  соединенным  с  ним  компьютерам.  Cеть
устроена так, что между любыми двумя компьютерами  есть  путь,
причем только один. Найти номера всех компьютеров,  с  которых
главный программист Гилл Бейтс может отправить сообщение  так,
чтобы максимальная задержка в  получении  сообщения  была  как
можно меньше.
  Ввод из файла INPUT.TXT. В первой строке вводится значение N
(1<=N<=10^5). В каждой из следующих N-1  строк  вводится через
пробел пара номеров компьютеров, обозначающая соединение.
  Вывод в файл OUTPUT.TXT. В первой строке выводится количество
искомых компьютеров M. Во второй строке выдаются через пробел в
порядке возрастания номера искомых компьютеров.
  Время счета не должно превышать 2 сек.
  Пример
  Ввод
4
1 2
4 3
2 3
  Вывод
2
2 3
  Указание. Предложить структуру данных, обеспечивающую быстрое
нахождение листьев бескорневого дерева из условия задачи (14).
Выполнил: Веселов Максим ПС-21
IDE: VS Code
C++ 17
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include <set>

void print(std::vector<int>* input) {
    for (auto n: *input) {
        std::cout << n << ' ';
    }
    std::cout << std::endl;
}

void print2(std::vector<std::vector<int>*> input) {
    int i = 0;
    for (auto v : input) {
        std::cout<<i++<<": [";
        print(v);
        std::cout <<"]"<<std::endl;
    }
    std::cout << std::endl;
}

std::vector<int> deletedPetals1(std::vector<std::vector<int>*>& adj_list, std::vector<int>& deleted_petals) {
    std::vector<int> petals;
    for (int i = 1; i < adj_list.size(); i ++){
        if (deleted_petals[i] == 1) {
            continue;
        }
        if (adj_list[i]->size() == 1) {
            petals.push_back(i);
            deleted_petals[i] = 1;
            adj_list[i]->erase(adj_list[i]->begin());
            for (int j = 1; j < adj_list.size(); j++){
                int index = -1;
                for (int k = 0; k < adj_list[j]->size(); k++) {
                    if (adj_list.at(j)->at(k) == i) {
                        index = k;
                        break;
                    }
                }
                std::vector<int>* a = adj_list[j];
                if (index == -1) {
                    continue;
                }
                a->erase(std::next(a->begin(), index));
            }
        }
    }

    return petals;
}

std::vector<int> deletedPetals(std::vector<std::vector<int>*>& adj_list, std::vector<int>& deleted_petals) {
    std::vector<int> petals;

    for (int i = 1; i < adj_list.size(); ++i) {
        if (deleted_petals[i] == 1) {
            continue;
        }
        if (adj_list[i]->size() <= 1) {
            petals.push_back(i);
        }
    }

    for (auto i: petals) {
        int neighbor = (*adj_list[i])[0];
        deleted_petals[i] = 1;

        // Удаление текущего узла из соседних списков
        for (int j = 1; j < adj_list.size(); ++j)
            adj_list[j]->erase(std::remove(adj_list[j]->begin(), adj_list[j]->end(), i), adj_list[j]->end());

        // Удаление текущего узла из соседнего списка
        adj_list[neighbor]->erase(std::remove(adj_list[neighbor]->begin(), adj_list[neighbor]->end(), i), adj_list[neighbor]->end());
    }
    return petals;
}


std::vector<int> find_desired_computers(int N, const std::vector<std::pair<int, int>>& connections) {
    std::vector<std::vector<int>*> adj_list;
    for (int i = 0 ; i <= N; i++) {
        std::vector<int>* m = new std::vector<int>;
        adj_list.push_back(m);
    }

    for (const auto& connection : connections) {
        int a = connection.first;
        int b = connection.second;
        adj_list[a]->push_back(b);
        adj_list[b]->push_back(a);
    }

    std::vector<int> deleted_petals(N + 1, 0);

    std::vector<int> petals = {};

    while (true) {
        print2(adj_list);
        std::cout<<"-- --- --- --- --\n";
        petals = deletedPetals(adj_list, deleted_petals);
        if (petals.size() <= 2) {
            break;
        }
    }

    return petals;
}

int main() {
    std::ifstream input("INPUT.TXT");
    std::ofstream output("OUTPUT.TXT");

    int N;
    input >> N;

    std::vector<std::pair<int, int>> connections(N - 1);
    for (int i = 0; i < N - 1; i++) {
        input >> connections[i].first >> connections[i].second;
    }

    std::vector<int> desired_computers = find_desired_computers(N, connections);

    output << desired_computers.size() << std::endl;
    for (int computer : desired_computers) {
        output << computer << ' ';
    }
    output << std::endl;

    return 0;
}