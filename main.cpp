/*
23. В текстовом файле записан  отдельный  абзац.  Некоторые
слова перенесены со строки на следующую строку.  Знак переноса
'-'.  Выровнять строки абзаца по ширине. Иными словами, правые
границы  строк  выравниваются  по  заданной  позиции  за  счет
вставки дополнительных пробелов между словами.  Первая  строка
абзаца должна иметь заданный отступ, а остальные строки должны
начинаться  с  первой  позиции.  Последняя  строка  абзаца  по
правому  краю  не  выравнивается. Переносы в выходном файле не
        допускаются.  Число  строк в исходном и сконечном файлах может
отличаться (9).

Выполнил: Веселов Максим ПС-21
IDE: CLion
C++ 17
*/


#include<iostream>
#include<fstream>
#include<clocale>
#include<string>
#include<vector>
#include<codecvt>
#include<io.h>
#include<fcntl.h>
#include<windows.h>
#include<cmath>

#define ERROR_COUNT_CMD_ARG_MESSAGE L"Ошибка: неверное количество аргументов.\nПример команды: Lab1.exe input.txt output.txt 20"
#define ERROR_OPEN_FILE L"Не удалось открыть файл"
#define ERROR_ARG_COUNT L"Неверно введен парметра \"Количестов символов в строке\""
#define ERROR_ARG_COUNT_IN_FIRST_LINE L"Неверно введен параметр \"Количество пробелов в первой строке\""

#define WORDS_SEPARATOR ' '
#define LINE_SEPARATOR '\n'
#define DASH '-'

bool is_number(const std::wstring &s) {
    for (wchar_t ch: s) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return true;
}

bool isWordSeparator(wchar_t ch) { return ch == WORDS_SEPARATOR; }

bool isLineSeparator(wchar_t ch) { return ch == LINE_SEPARATOR; }

bool isDash(wchar_t ch) { return ch == DASH; }

std::wstring readWord(std::wifstream &in_f) {
    std::wstring word;
    wchar_t ch;
    wchar_t ch1;

    while (!(in_f.get(ch), in_f.eof())) {
        if (isWordSeparator(ch) || (isLineSeparator(ch) && !isDash(ch1))) {
            break;
        }

        if (isDash(ch1) && !isLineSeparator(ch)) {
            word += ch1;
        }

        if (!((isLineSeparator(ch) && isDash(ch1)) || isDash(ch))) {
            word += ch;
        }

        ch1 = ch;
    }
    return word;
}

int lengthWords(std::vector<std::wstring> &words) {
    int length = 0;
    for (const std::wstring &word: words) {
        length += (int) word.length();
    }
    return length;
}

int findCountSpace(int freeCountSpace, int emptyCountSectors) {
    if (!freeCountSpace || !emptyCountSectors) {
        return 0;
    }
    return std::ceil((float) freeCountSpace / (float) emptyCountSectors);
}

std::wstring getSpaces(int countSpaces) {
    std::wstring result;
    std::wstring space = L" ";
    for (int i = 0; i < countSpaces; i++) {
        result.append(space);
    }
    return result;
}

std::vector<std::wstring> splitWord(std::wstring word, int split_index) {
    std::vector<std::wstring> pair;
    int word_len = word.length();
    std::wstring first_part;
    std::wstring second_part;
    for (int i = 0; i < word_len; i++) {
        if (i < split_index) {
            first_part += word[i];
        } else {
            second_part += word[i];
        }
    }
    pair.push_back(first_part);
    pair.push_back(second_part);
    return pair;
}

std::wstring alignWords(std::vector<std::wstring> &words, int count_in_line) {
    if (words.size() == 1) {
        std::wstring word = words[0];
        if (word.length() >= count_in_line) {
            std::vector<std::wstring> pair = splitWord(word, count_in_line);
            words[0] = pair[1];
            return pair[0];
        }
        return word;

    }
    std::wstring result;
    int words_count = words.size();
    int length = (int) lengthWords(words) - words[words.size() - 1].length();
    int count_sections = (words.size() - 1) - 1;
    int free_spaces = count_in_line - length;
    for (int i = 0; i < words_count; i++) {
        if (words_count != 1 && words.size() <= 1) {
            break;
        }
        std::wstring word = words[0];
        words.erase(words.begin());
        if (word.length() >= count_in_line) {
            std::vector<std::wstring> pair = splitWord(word, free_spaces);
            result.append(pair[0]);
            words[0] = pair[1];
            break;
        }
        result.append(word);
        int count_spaces = findCountSpace(free_spaces, count_sections);
        std::wcout << word << " " << count_spaces << "\n";
        result.append(getSpaces(count_spaces));
        free_spaces -= count_spaces;
        count_sections--;
    }
    return result;
}

std::wstring wordsToString(std::vector<std::wstring> words) {
    std::wstring result;
    for (std::wstring &word: words) {
        result.append(word);
        result += WORDS_SEPARATOR;
    }
    return result;
}

void readOneLine(std::vector<std::wstring> &old_words, std::wifstream &in_f, int count) {
    int words_len = lengthWords(old_words);
    while (!(in_f.eof()) && (words_len + int(old_words.size()) - 1 <= count)) {
        std::wstring word = readWord(in_f);
        if ((int) word.size()) {
            words_len += (int) word.size();
            old_words.push_back(word);
        }
    }
}

void processingLine(std::wifstream &in_f, std::wofstream &out_f, std::vector<std::wstring> &words, int count_in_line, int count_space_in_first = 0) {
    out_f << getSpaces(count_space_in_first);
    readOneLine(words, in_f, count_in_line);
    if (in_f.eof()) {
        out_f << wordsToString(words) << LINE_SEPARATOR;
        return;
    }
    out_f << alignWords(words, count_in_line - count_space_in_first) << LINE_SEPARATOR;
}

void processingFile(std::wifstream &in_f, std::wofstream &out_f, int count_in_line, int count_space_in_first) {
    std::vector<std::wstring> words;
    if (!in_f.eof()) {
        processingLine(in_f, out_f, words, count_in_line, count_space_in_first);
    }
    while (!in_f.eof()) {
        processingLine(in_f, out_f, words, count_in_line);
    }
}

int main() {
    int argc;
    wchar_t **argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    std::locale utf8_locale(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::locale::global(utf8_locale);


    if (argc != 5) {
        std::wcout << ERROR_COUNT_CMD_ARG_MESSAGE;
        exit(-1);
    }

    std::wstring input_filename(argv[1]);
    std::wstring output_filename(argv[2]);

    std::wstring count_arg = argv[3];
    if (!is_number(count_arg) || _wtoi(argv[3]) <= 0) {
        std::wcout << ERROR_ARG_COUNT;
        exit(-1);
    }
    int count_in_line = _wtoi(argv[3]);

    std::wstring count_space_in_first_arg = argv[4];
    if (!is_number(count_space_in_first_arg) || _wtoi(argv[4]) <= 0|| _wtoi(argv[4]) > count_in_line) {
        std::wcout << ERROR_ARG_COUNT_IN_FIRST_LINE;
        exit(-1);
    }
    int count_space_in_first = _wtoi(argv[4]);


    std::wifstream in_f;
    in_f.open(argv[1]);
    if (!in_f.is_open()) {
        std::wcout << ERROR_OPEN_FILE;
        exit(-1);
    }

    std::wofstream out_f(argv[2]);
    if (!out_f.is_open()) {
        std::wcout << ERROR_OPEN_FILE;
        exit(-1);
    }

    processingFile(in_f, out_f, count_in_line, count_space_in_first);

    in_f.close();
    out_f.close();
    return 0;
}
