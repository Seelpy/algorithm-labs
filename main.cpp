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
#include <chrono>

#include <set>
#include <map>

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

std::vector<int> deletedPetals2(std::vector<std::vector<int>*>& adj_list, std::vector<int>& deleted_petals) {
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

std::map<int, std::vector<int>*> deletedPetals(std::map<int, std::vector<int>*>* adj_list, std::map<int, std::vector<int>*>* old_petals) {
    std::vector<int> petals;
    std::map<int, std::vector<int>*> feature_petals;
    std::vector<std::pair<int, int>> petals_on_delete;

    std::map<int, std::vector<int>*>* now_petals;
    if (old_petals != nullptr) {
        now_petals = old_petals;
    } else {
        now_petals = adj_list;
    }

    for (const auto& pair: *now_petals) {
        if (pair.second->size() <= 1) {
            petals.push_back(pair.first);
            if (!pair.second->empty()) {
                int tmp_id  = (*pair.second)[0];
                feature_petals[tmp_id] = (*adj_list)[tmp_id];
                petals_on_delete.push_back(std::pair<int, int>{pair.first, tmp_id});
            }
        }
    }

    for (auto p: petals_on_delete) {
        auto list = (*adj_list)[p.second];
        auto newEnd = std::remove(list->begin(), list->end(), p.first);
        list->erase(newEnd, list->end());
    }

    for (auto id: petals) {
        auto it = adj_list->find(id);
        if (it != adj_list->end()) {
            adj_list->erase(it);
        }
    }


    return feature_petals;
}




std::vector<int> find_desired_computers(int N, const std::vector<std::pair<int, int>>& connections) {
    std::map<int, std::vector<int>*> adj_list;
    for (int i = 1; i <= N; i++) {
        auto m = new std::vector<int>;
        adj_list[i] = m;
    }

    for (const auto& connection : connections) {
        int a = connection.first;
        int b = connection.second;
        adj_list[a]->push_back(b);
        adj_list[b]->push_back(a);
    }


    std::map<int, std::vector<int>*> petals = {};
    int i = 1;
    while (true) {
        petals = deletedPetals(&adj_list, petals.empty() ? nullptr: &petals);

        if (adj_list.size() <= 2) {
            break;
        }

        if (++i % 500 == 0 ) {
            std::cout<<"I: " << i<< std::endl;
        }
    }

    std::vector<int> petals_ids;

    // Iterate through the map and store keys in the vector
    for (const auto& pair : adj_list) {
        petals_ids.push_back(pair.first);
    }

    return petals_ids;
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

    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<int> desired_computers = find_desired_computers(N, connections);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Выводим время выполнения
    std::cout << "Time work function: " << duration.count() << " millisecond\n";

    output << desired_computers.size() << std::endl;
    for (int computer : desired_computers) {
        output << computer << ' ';
    }
    output << std::endl;

    return 0;
}