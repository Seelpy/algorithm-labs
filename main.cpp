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
#include <set>

int findMax(const std::vector<int>& numbers) {
    int max = numbers[0];

    for (int i = 1; i < numbers.size(); ++i) {
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    return max;
}

std::vector<int> findIds(const std::vector<int>& numbers, int max) {
    std::vector<int> ids;

    for (int i = 1; i <= numbers.size(); ++i) {
        if (numbers[i] == max) {
            ids.push_back(i);
        }
    }

    return ids;
}

int findSecondMax(const std::vector<int>& numbers) {
    int max = 0;
    int second_max = 0;

    for (int i = 1; i < numbers.size(); ++i) {
        if (numbers[i] > max) {
            second_max = max;
            max = numbers[i];
        } else if (numbers[i] > second_max && numbers[i] != max) {
            second_max = numbers[i];
        }
    }

    return second_max;
}

void dfs(int node, const std::vector<std::vector<int>>& adj_list, std::vector<bool>& visited, std::vector<int>& node_distances, int distance, int& farthest_node) {
    visited[node] = true;

    node_distances[node] = distance;

    for (int neighbor : adj_list[node]) {
        if (!visited[neighbor]) {
            int new_distance = distance + 1;
            int new_farthest_node = farthest_node;
            dfs(neighbor, adj_list, visited, node_distances, new_distance, new_farthest_node);
        }
    }
}

int getFather(int node, const std::vector<std::vector<int>>& adj_list, std::vector<int> distances) {
    int id;
    for (int neighbor : adj_list[node]) {
        if (distances[neighbor] < distances[node]) {
            return neighbor;
        }
    }
    std::cout<<"ERROR";
    return -1;
}

std::vector<int> backwardDfs(int node, const std::vector<std::vector<int>>& adj_list, std::vector<int> distances) {
    std::vector<int> track;
    track.push_back(node);
    while (distances[node] != 0) {
        int father = getFather(node, adj_list, distances);
        track.push_back(father);
        node = father;
    }

    return track;
}
void print(std::vector<int> const &input) {
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
    std::cout << std::endl;
}

std::vector<int> findPetals(const std::vector<std::vector<int>>& adj_list, std::vector<int> distances) {
    std::vector<int> petals;
    petals.push_back(0);
    for (int i = 1; i <= adj_list.size(); i ++){
        if (adj_list[i].size() == 1) {
            petals.push_back(distances[i]);
        } else {
            petals.push_back(0);
        }
    }
    return petals;
}



std::vector<int> find_desired_computers(int N, const std::vector<std::pair<int, int>>& connections) {
    std::vector<std::vector<int>> adj_list(N + 1);

    for (const auto& connection : connections) {
        int a = connection.first;
        int b = connection.second;
        adj_list[a].push_back(b);
        adj_list[b].push_back(a);
    }

    std::vector<int> desired_computers;

    std::vector<bool> visited(N + 1, false);
    std::vector<int> distances(N + 1, 0);
    int distance = 0;
    int farthest_node = 1;
    dfs(farthest_node, adj_list, visited, distances, distance, farthest_node);
    std::vector<int> petals = findPetals(adj_list, distances);

    int max = findMax(petals);
    int second_max = findSecondMax(petals);
    int dif = max - second_max;

    std::vector<int> ids = findIds(petals, max);
    for (const auto& node: ids) {
        std::vector<int> track = backwardDfs(node, adj_list, distances);
        if (dif % 2 == 0) {
            desired_computers.push_back(track[track.size()  - 1 - dif/2]);
        } else {
            desired_computers.push_back(track[track.size()  - 1 - (dif + 1)/2]);
            desired_computers.push_back(track[track.size()  - 1 - (dif - 1)/2]);
        }
    }

    std::set<int> s(desired_computers.begin(), desired_computers.end());

    return std::vector<int>(s.begin(), s.end());
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