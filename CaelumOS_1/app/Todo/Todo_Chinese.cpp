#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <windows.h>
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 0x07);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    json j;
    ifstream fin("../system/todo.json");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (fin.is_open()) {
        fin >> j;
        fin.close();
    } else {
        j = json::array();
    }
    int n;
    SetConsoleTextAttribute(hConsole, 0x0B);
    printf("====待办事项列表====\n");
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
    SetConsoleTextAttribute(hConsole, 0x0D);
    printf("\n1.添加新事项\n2.更改事项完成情况\n3.清空所有事项\n4.打印所有事项\n5.删除事项\n6.编辑事项\n0.退出\n> ");
    SetConsoleTextAttribute(hConsole, 0x07);
    while (true) {
        scanf("%d", &n);
        {
            string dummy;
            getline(cin, dummy);
        }
        if (n == 1) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("请输入新事项：\n");
            SetConsoleTextAttribute(hConsole, 0x07);
            string new_item;
            getline(cin, new_item);
            j.push_back({new_item, false});
            ofstream fout("../system/todo.json");
            if (!fout.is_open()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("保存文件失败!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                return 0;
            }
            fout << j.dump(4);
            fout.close();
            SetConsoleTextAttribute(hConsole, 0x0A);
            printf("已保存！\n");
            SetConsoleTextAttribute(hConsole, 0x07);
        } else if (n == 2) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("请输入要更改的事项编号：\n");
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
                    printf("保存文件失败!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("状态已切换!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("无效的编号\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 3) {
            j.clear();
            ofstream fout("../system/todo.json");
            if (!fout.is_open()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("保存文件失败!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                return 0;
            }
            fout << j.dump(4);
            fout.close();
            SetConsoleTextAttribute(hConsole, 0x0A);
            printf("所有事项已清空！\n");
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
            printf("请输入要删除的事项编号：\n");
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
                    printf("保存文件失败!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("事项已删除!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("无效的编号\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 6) {
            SetConsoleTextAttribute(hConsole, 0x09);
            printf("请输入要编辑的事项编号：\n");
            SetConsoleTextAttribute(hConsole, 0x07);
            scanf("%d", &n);
            {
                string dummy;
                getline(cin, dummy);
            }
            if (n >= 1 && n <= j.size()) {
                SetConsoleTextAttribute(hConsole, 0x09);
                printf("请输入新的事项内容：\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                string new_item;
                getline(cin, new_item);
                j[n - 1][0] = new_item;
                ofstream fout("../system/todo.json");
                if (!fout.is_open()) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("保存文件失败!\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    return 0;
                }
                fout << j.dump(4);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("事项已编辑!\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 0) {
            SetConsoleTextAttribute(hConsole, 0x07);
            return 0;
        }
    }
}