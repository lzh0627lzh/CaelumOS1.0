#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <windows.h>
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

int main() {HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x07);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    json j;
    ifstream fin("../system/todo.json");
    if (fin.is_open()) {
        fin >> j;
        fin.close();
    } else {
        j = json::array();
    }
    int n;
    SetConsoleTextAttribute(hConsole, 0x0B);
    printf("====To‑Do List====\n");
    for (int i = 0; i < j.size(); i++) {
        string text = j[i][0].get<string>();
        bool done = j[i][1].get<bool>();
        if (done) {
            SetConsoleTextAttribute(hConsole, 0x0A);
            printf("%d. [√] %s\n", i + 1, text.c_str());
        } else {
            SetConsoleTextAttribute(hConsole, 0x0E);
            printf("%d. [ ] %s\n", i + 1, text.c_str());
        }
    }
    while (true) {
        SetConsoleTextAttribute(hConsole, 0x0D);
        printf("\n1.Add new item\n2.Toggle item completion\n3.Clear all items\n4.Print all items\n5.Delete item\n6.Edit item\n0.Exit\n> ");
        SetConsoleTextAttribute(hConsole, 0x07);
        scanf("%d", &n);
        {
            string dummy;
            getline(cin, dummy);
        }
        if (n == 1) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("Please enter new item: \n");
            SetConsoleTextAttribute(hConsole, 0x07);
            string new_item;
            getline(cin, new_item);
            j.push_back({new_item, false});
            ofstream fout("../system/todo.json");
            if (!fout.is_open()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Save file failed!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                return 0;
            }
            fout << j.dump(4);
            fout.close();
            SetConsoleTextAttribute(hConsole, 0x0A);
            printf("Saved!\n");
            SetConsoleTextAttribute(hConsole, 0x07);
        } else if (n == 2) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("Please enter the item number to toggle: \n");
            SetConsoleTextAttribute(hConsole, 0x07);
            scanf("%d", &n);
            {
                string dummy;
                getline(cin, dummy);
            }
            if (n >= 1 && n <= j.size()) {
                j[n - 1][1] = !j[n - 1][1];
                ofstream fout("../system/todo.json");
                if (!fout.is_open()) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("Save file failed!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("Status toggled!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Invalid number\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 3) {
            j.clear();
            ofstream fout("../system/todo.json");
            if (!fout.is_open()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Save file failed!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                return 0;
            }
            fout << j.dump(4);
            fout.close();
            SetConsoleTextAttribute(hConsole, 0x0A);
            printf("All items cleared!\n");
            SetConsoleTextAttribute(hConsole, 0x07);
        } else if (n == 4) {
            for (int i = 0; i < j.size(); i++) {
                string text = j[i][0].get<string>();
                bool done = j[i][1].get<bool>();
                if (done) {
                    SetConsoleTextAttribute(hConsole, 0x0A);
                    printf("%d. [√] %s\n", i + 1, text.c_str());
                } else {
                    SetConsoleTextAttribute(hConsole, 0x0E);
                    printf("%d. [ ] %s\n", i + 1, text.c_str());
                }
            }
        } else if (n == 5) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("Please enter the item number to delete: \n");
            SetConsoleTextAttribute(hConsole, 0x07);
            scanf("%d", &n);
            {
                string dummy;
                getline(cin, dummy);
            }
            if (n >= 1 && n <= j.size()) {
                j.erase(j.begin() + n - 1);
                ofstream fout("../system/todo.json");
                if (!fout.is_open()) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("Save file failed!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("Item deleted!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Invalid number\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 6) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("Please enter the item number to edit: \n");
            SetConsoleTextAttribute(hConsole, 0x07);
            scanf("%d", &n);
            {
                string dummy;
                getline(cin, dummy);
            }
            if (n >= 1 && n <= j.size()) {
                SetConsoleTextAttribute(hConsole, 0x09);
                printf("Please enter the new item content: \n");
                SetConsoleTextAttribute(hConsole, 0x07);
                string new_item;
                getline(cin, new_item);
                j[n - 1][0] = new_item;
                ofstream fout("../system/todo.json");
                if (!fout.is_open()) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("Save file failed!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("Item edited!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("Invalid number\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 0) {
            system("cls");
            SetConsoleTextAttribute(hConsole, 0x07);
            return 0;
        }
    }
}