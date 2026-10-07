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
        printf("当前文件个数：%d\n", count_json_files("..\\..\\file"));
        printf("1.创建新文件\n");
        printf("2.删除文件\n");
        printf("3.打开文件(查看)\n");
        printf("4.编辑文件\n");
        printf("0.退出\n>");
        scanf("%d", &n);
        {
            string dummy;
            getline(cin, dummy);
        }
        SetConsoleTextAttribute(hConsole, 0x07);
        if (n == 1) {
            string filename;
            printf("请输入文件名（不带后缀）：");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件名不能为空！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (!fs::exists("..\\..\\file")) {
                fs::create_directories("..\\..\\file");
            }
            if (fs::exists(filepath)) {
                SetConsoleTextAttribute(hConsole, 0x0E);
                printf("警告：该文件已经存在，请勿重复创建！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            ofstream fout(filepath);
            if (fout.is_open()) {
                json arr = json::array();
                fout << arr.dump(2);
                fout.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("文件创建成功！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件创建失败！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 2) {
            string filename;
            printf("请输入要删除的文件名（不带后缀）：");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件名不能为空！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (fs::exists(filepath)) {
                fs::remove(filepath);
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("文件删除成功！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件不存在！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 3) {
            string filename;
            printf("请输入要打开查看的文件名（不带后缀）：");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件名不能为空！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (fs::exists(filepath)) {
                std::ifstream fin(filepath);
                json j;
                try {
                    fin >> j;
                    SetConsoleTextAttribute(hConsole, 0x0A);
                    printf("成功加载！文本预览：\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    printf("==========文档内容==========\n");
                    for (int i = 0; i < j.size(); i++) {
                        printf("%s\n", j[i].get<string>().c_str());
                    }
                    printf("============================\n");
                } catch (nlohmann::json::parse_error &e) {
                    SetConsoleTextAttribute(hConsole, 0x0C);
                    printf("JSON解析出错：%s\n", e.what());
                    SetConsoleTextAttribute(hConsole, 0x07);
                }
                fin.close();
            } else {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件不存在！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 4) {
            string filename;
            printf("请输入要编辑的文件名（不带后缀）：");
            getline(cin, filename);
            if (filename.empty()) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件名不能为空！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            string filepath = "..\\..\\file\\" + filename + ".json";
            if (!fs::exists(filepath)) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("文件不存在！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
                continue;
            }
            ifstream fin(filepath);
            json j;
            try {
                fin >> j;
            } catch (nlohmann::json::parse_error &e) {
                SetConsoleTextAttribute(hConsole, 0x0C);
                printf("解析错误：%s\n", e.what());
                SetConsoleTextAttribute(hConsole, 0x07);
                fin.close();
                continue;
            }
            fin.close();
            if (!j.is_array()) {
                j = json::array();
            }
            printf("\n====当前文档内容====\n");
            for (int i = 0; i < j.size(); i++) {
                printf("%s\n", j[i].get<string>().c_str());
            }
            printf("====================\n");
            int sel;
            printf("\n选择编辑模式：\n");
            printf("1 修改指定行\n");
            printf("2 全部重写\n");
            printf("3 末尾追加新行\n>");
            scanf("%d", &sel);
            {
                string dummy2;
                getline(cin, dummy2);
            }
            if (sel == 1) {
                int line_no;
                printf("请输入要修改的行号(从1开始)：");
                scanf("%d", &line_no);
                {
                    string dummy2;
                    getline(cin, dummy2);
                }
                if (line_no < 1 || line_no > (int)j.size()) {
                    SetConsoleTextAttribute(hConsole, 0x0E);
                    printf("行号越界！\n");
                    SetConsoleTextAttribute(hConsole, 0x07);
                    continue;
                }
                string newtext;
                printf("输入该行新内容：");
                getline(cin, newtext);
                j[line_no - 1] = newtext;
            } else if (sel == 2) {
                j.clear();
                printf("开始输入新全部内容；输入单独一行 `@@end`结束录入\n");
                while (true) {
                    string s;
                    getline(cin, s);
                    if (s == "@@end")
                        break;
                    j.push_back(s);
                }
            } else if (sel == 3) {
                string add_text;
                printf("请输入追加到末尾的新一行文本：");
                getline(cin, add_text);
                j.push_back(add_text);
                ofstream fsave(filepath);
                fsave << j.dump(2);
                fsave.close();
                SetConsoleTextAttribute(hConsole, 0x0A);
                printf("保存成功！\n");
                SetConsoleTextAttribute(hConsole, 0x07);
            }
        } else if (n == 0) {
            break;
        } else {
            SetConsoleTextAttribute(hConsole, 0x0C);
            printf("无效的选项，请重新选择。\n");
            SetConsoleTextAttribute(hConsole, 0x07);
        }
    }
    return 0;
}