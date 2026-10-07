#include <bits/stdc++.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <windows.h>
using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;
int n;

int count_json_files(const std::string &folder_path) {
    int cnt = 0;
    if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
        return 0;
    }
    for (const auto &entry : fs::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension() == ".json") {
                cnt++;
            }
        }
    }
    return cnt;
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x07);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    while (true) {
        SetConsoleTextAttribute(hConsole, 0x03);
        printf("Total files: %d\n", count_json_files("..\\..\\file"));
        printf("1. Create new file\n");
        printf("2. Delete file\n");
        printf("3. View file\n");
        printf("4. Edit file\n");
        printf("0. Exit\n>");
        scanf("%d", &n);
        {
            string dummy;
            getline(cin, dummy);
        }
        SetConsoleTextAttribute(hConsole, 0x07);

        if (n == 1) {
            string filename;
            printf("Input file name(without suffix): ");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File name cannot be empty!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (!fs::exists("..\\..\\file")) {
                fs::create_directories("..\\..\\file");
            }
            if (fs::exists(filepath)) {
                SetConsoleTextAttribute(hConsole, 0x0E);
                printf("Warning: file already exists!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            ofstream fout(filepath);
            if (fout.is_open()) {
                json arr = json::array();
                fout << arr.dump(2);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("File created successfully!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Failed to create file!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 2) {
            string filename;
            printf("Input file name to delete(without suffix): ");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File name cannot be empty!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (fs::exists(filepath)) {
                fs::remove(filepath);
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("File deleted successfully!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File does not exist!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 3) {
            string filename;
            printf("Input file name to view(without suffix): ");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File name cannot be empty!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (fs::exists(filepath)) {
                std::ifstream fin(filepath);
                json j;
                try {
                    fin >> j;
                    if (!j.is_array()) j = json::array();
                    SetConsoleTextAttribute(hConsole, 0x0A);
                    printf("Loaded! Text preview:\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    printf("==========Document==========\n");
                    for (int i = 0; i < j.size(); i++) {
                        printf("%s\n", j[i].get<string>().c_str());
                    }
                    printf("============================\n");
                } catch (nlohmann::json::parse_error &e) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("JSON parse error: %s\n", e.what());
                    SetConsoleTextAttribute(hConsole, 0x07);
                }
                fin.close();
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File does not exist!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 4) {
            string filename;
            printf("Input file name to edit(without suffix): ");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File name cannot be empty!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (!fs::exists(filepath)) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("File does not exist!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            ifstream fin(filepath);
            json j;
            try {
                fin >> j;
            } catch (nlohmann::json::parse_error &e) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Parse error: %s\n", e.what());
                SetConsoleTextAttribute(hConsole, 0x07);
                fin.close();
                continue;
            }
            fin.close();

            if (!j.is_array()) {
                j = json::array();
            }

            printf("\n====Current document====\n");
            for (int i = 0; i < j.size(); i++) {
                printf("%s\n", j[i].get<string>().c_str());
            }
            printf("========================\n");

            int sel;
            printf("\nSelect edit mode:\n");
            printf("1 Modify specified line\n");
            printf("2 Rewrite all\n");
            printf("3 Append new line at end\n>");
            scanf("%d", &sel);
            {
                string dummy2;
                getline(cin, dummy2);
            }

            if (sel == 1) {
                int line_no;
                printf("Input line number(start from 1): ");
                scanf("%d", &line_no);
                {
                    string dummy2;
                    getline(cin, dummy2);
                }
                if (line_no < 1 || line_no > (int)j.size()) {
                    SetConsoleTextAttribute(hConsole, 0x0E);
                    printf("Line index out of range!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    continue;
                }
                string newtext;
                printf("Input new content for this line: ");
                getline(cin, newtext);
                j[line_no - 1] = newtext;
            } else if (sel == 2) {
                j.clear();
                printf("Enter new content; type @@end on separate line to finish\n");
                while (true) {
                    string s;
                    getline(cin, s);
                    if (s == "@@end")
                        break;
                    j.push_back(s);
                }
            } else if (sel == 3) {
                string add_text;
                printf("Input new line to append: ");
                getline(cin, add_text);
                j.push_back(add_text);
            } else {
                SetConsoleTextAttribute(hConsole,0x0C);
                printf("Invalid sub‑option.\n");
                SetConsoleTextAttribute(hConsole,0x07);
            }

            // unified save
            ofstream fsave(filepath);
            fsave << j.dump(2);
            fsave.close();
            SetConsoleTextAttribute(hConsole,0x0A);
            printf("Saved successfully!\n");
            SetConsoleTextAttribute(hConsole,0x07);
        } else if(n == 0) {
            break;
        } else {
            SetConsoleTextAttribute(hConsole,0x0C);
            printf("Invalid main menu option.\n");
            SetConsoleTextAttribute(hConsole,0x07);
        }
    }
    return 0;
}
