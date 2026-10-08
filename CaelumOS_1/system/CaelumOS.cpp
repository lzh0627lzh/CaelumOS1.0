#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <windows.h>
#define CAELUM_VERSION "V_1.4(1400.4812)"
using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;
const int MAX_APP = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void set_full_console_color(WORD attr) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    COORD start = {0, 0};
    DWORD total = info.dwSize.X * info.dwSize.Y;
    DWORD written;
    FillConsoleOutputAttribute(hConsole, attr, total, start, &written);
    FillConsoleOutputCharacter(hConsole, ' ', total, start, &written);
    SetConsoleTextAttribute(hConsole, attr);
}

struct AppItem {
    string name_zh;
    string name_en;
    string bin_zh;
    string bin_en;
    string version;
    string category;
    string author;
    string dir_path;
};
AppItem app_db[MAX_APP];
int app_count = 0;

bool get_required_string(const json& object, const string& key, string& value) {
    auto it = object.find(key);
    if (it == object.end() || !it->is_string())
        return false;
    value = it->get<string>();
    return true;
}

bool is_safe_executable_name(const string& value) {
    if (value.empty() || value.find('\0') != string::npos)
        return false;

    fs::path path(value);
    if (path.is_absolute() || path.has_root_name() ||
        path.has_root_directory() || !path.parent_path().empty())
        return false;

    string extension = path.extension().string();
    transform(extension.begin(), extension.end(), extension.begin(),
              [](unsigned char ch) { return static_cast<char>(tolower(ch)); });
    return extension == ".exe";
}

bool launch_app(const AppItem& app, const string& executable_name) {
    if (!is_safe_executable_name(executable_name)) {
        cerr << "应用程序路径无效 / Invalid application executable path\n";
        return false;
    }

    std::error_code ec;
    fs::path directory = fs::canonical(fs::path(app.dir_path), ec);
    if (ec) {
        cerr << "无法访问应用目录 / Cannot access app directory: "
             << ec.message() << '\n';
        return false;
    }

    fs::path executable =
        fs::canonical(directory / fs::path(executable_name), ec);
    if (ec || executable.parent_path() != directory ||
        !fs::is_regular_file(executable, ec) || ec) {
        cerr << "应用可执行文件不存在或不安全 / App executable is missing or unsafe\n";
        return false;
    }

    wstring command_line = L"\"" + executable.wstring() + L"\"";
    vector<wchar_t> mutable_command_line(command_line.begin(), command_line.end());
    mutable_command_line.push_back(L'\0');

    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(executable.c_str(), mutable_command_line.data(),
                        nullptr, nullptr, FALSE, 0, nullptr, directory.c_str(),
                        &startup, &process)) {
        cerr << "启动应用失败 / Failed to launch app (Windows error "
             << GetLastError() << ")\n";
        return false;
    }

    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return true;
}

bool load_app(const fs::path& json_path, AppItem& app) {
    ifstream fin(json_path);
    if (!fin.is_open()) {
        cerr << "无法打开应用配置 / Cannot open app config: "
             << json_path.string() << '\n';
        return false;
    }

    json j;
    try {
        fin >> j;
    } catch (const json::exception& e) {
        cerr << "应用配置 JSON 无效 / Invalid app config JSON: "
             << json_path.string() << " (" << e.what() << ")\n";
        return false;
    }

    if (!j.is_object()) {
        cerr << "应用配置顶层必须是对象 / App config must be an object: "
             << json_path.string() << '\n';
        return false;
    }

    auto name = j.find("name");
    auto bin = j.find("bin");
    if (name == j.end() || !name->is_object() ||
        bin == j.end() || !bin->is_object() ||
        !get_required_string(*name, "zh-CN", app.name_zh) ||
        !get_required_string(*name, "en-US", app.name_en) ||
        !get_required_string(*bin, "zh-CN", app.bin_zh) ||
        !get_required_string(*bin, "en-US", app.bin_en) ||
        !is_safe_executable_name(app.bin_zh) ||
        !is_safe_executable_name(app.bin_en) ||
        !get_required_string(j, "version", app.version) ||
        !get_required_string(j, "category", app.category) ||
        !get_required_string(j, "author", app.author)) {
        cerr << "应用配置字段缺失或类型错误 / Missing or invalid app config field: "
             << json_path.string() << '\n';
        return false;
    }

    return true;
}

void scan_app_folder() {
    app_count = 0;
    try {
        for (const auto& entry : fs::directory_iterator("../app")) {
            if (!entry.is_directory())
                continue;
            if (app_count >= MAX_APP) {
                cerr << "应用数量超过上限 " << MAX_APP
                     << " / App limit reached\n";
                break;
            }

            AppItem app{};
            fs::path jsonpath = entry.path() / "app.json";
            if (!load_app(jsonpath, app))
                continue;
            app.dir_path = entry.path().string();
            app_db[app_count++] = app;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "扫描应用目录失败 / Failed to scan app directory: "
             << e.what() << '\n';
    }
}

int lan;

bool read_integer(const string& prompt, int& value) {
    cout << prompt;
    string line;
    if (!getline(cin, line))
        return false;

    istringstream input(line);
    int parsed;
    char extra;
    if (!(input >> parsed) || (input >> extra))
        return false;

    value = parsed;
    return true;
}

bool split_command(const string& line, vector<string>& args) {
    args.clear();
    string current;
    bool in_quotes = false;
    bool token_started = false;

    for (char ch : line) {
        if (ch == '"') {
            in_quotes = !in_quotes;
            token_started = true;
        } else if (isspace(static_cast<unsigned char>(ch)) && !in_quotes) {
            if (token_started) {
                args.push_back(current);
                current.clear();
                token_started = false;
            }
        } else {
            current += ch;
            token_started = true;
        }
    }

    if (in_quotes)
        return false;
    if (token_started)
        args.push_back(current);
    return true;
}

bool parse_hex_color(const string& text, WORD& color) {
    const char* first = text.data();
    const char* last = first + text.size();
    if (text.size() >= 2 && text[0] == '0' &&
        (text[1] == 'x' || text[1] == 'X')) {
        first += 2;
    }
    if (first == last)
        return false;

    unsigned int value = 0;
    auto result = from_chars(first, last, value, 16);
    if (result.ec != errc() || result.ptr != last || value > 0xFF)
        return false;
    color = static_cast<WORD>(value);
    return true;
}

namespace cpu {
void Time() {
    time_t t = time(NULL);
    tm lt{};
    localtime_s(&lt, &t);
    printf("%04d-%02d-%02d ", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
    printf("%02d:%02d:%02d\n", lt.tm_hour, lt.tm_min, lt.tm_sec);
}
void System_Exit() {
    SetConsoleTextAttribute(hConsole, 0x09);
    if (lan == 1) {
        printf("系统即将关闭\n");
    } else {
        printf("System is shutting down...\n");
    }
    SetConsoleTextAttribute(hConsole, 0x07);
    Sleep(1000);
    exit(1);
}
void Help() {
    if (lan == 1) {
        SetConsoleTextAttribute(hConsole, 0x0E);
        printf("====命令帮助====\n");
        printf("app                 打开简易应用列表\n");
        printf("store               打开应用中心\n");
        printf("time                查看系统时间\n");
        printf("clean               清屏\n");
        printf("color 0F            修改后续输出文字颜色(临时)\n");
        printf("theme 17            修改整个终端背景+文字(永久保存)\n");
        printf("system information  查看系统信息\n");
        printf("system changelog    查看更新日志\n");
        printf("exit                关闭Caelum OS\n");
        printf("todo                打开待办事项列表\n");
        SetConsoleTextAttribute(hConsole, 0x07);
    } else {
        SetConsoleTextAttribute(hConsole, 0x0E);
        printf("===============Command Help===============\n");
        printf("app                 open simple app list\n");
        printf("store               open app center\n");
        printf("time                show current time\n");
        printf("clean               clear screen\n");
        printf("color 0F            change following text color(temp)\n");
        printf("theme 17            change whole console bg+fg(saved)\n");
        printf("system information  view system information\n");
        printf("system changelog    view changelog\n");
        printf("exit                shutdown Caelum OS\n");
        printf("todo                open todo list\n");
        SetConsoleTextAttribute(hConsole, 0x07);
    }
}
void Color(const vector<string>& args) {
    WORD color;
    if (args.size() != 2 || !parse_hex_color(args[1], color)) {
        SetConsoleTextAttribute(hConsole, 0x0C);
        if (lan == 1)
            printf("用法：color 0B（颜色值必须是 00-FF 的十六进制数）\n");
        else
            printf("Usage: color 0B (color must be hexadecimal 00-FF)\n");
        SetConsoleTextAttribute(hConsole, 0x07);
        return;
    }
    SetConsoleTextAttribute(hConsole, color);
}

void Theme(const vector<string>& args) {
    WORD attr;
    if (args.size() != 2 || !parse_hex_color(args[1], attr)) {
        SetConsoleTextAttribute(hConsole, 0x0C);
        if (lan == 1)
            printf("用法：theme 17（颜色值必须是 00-FF 的十六进制数）\n");
        else
            printf("Usage: theme 17 (color must be hexadecimal 00-FF)\n");
        SetConsoleTextAttribute(hConsole, 0x07);
        return;
    }
    set_full_console_color(attr);
    json j_out;
    j_out["console_color"] = (int)attr;
    ofstream fout("../system/color.json");
    if (!fout.is_open()) {
        SetConsoleTextAttribute(hConsole, 0x0C);
        if (lan == 1)
            printf("保存颜色配置失败！\n");
        else
            printf("Failed saving color config!\n");
        SetConsoleTextAttribute(hConsole, 0x07);
        return;
    }
    fout << j_out.dump(4);
    fout.close();
    SetConsoleTextAttribute(hConsole, 0x0A);
    if (lan == 1)
        printf("主题颜色已经设置并保存！\n");
    else
        printf("Theme color applied and saved!\n");
    SetConsoleTextAttribute(hConsole, 0x07);
}

void System_Information() {
    if (lan == 1) {
        printf("系统名称：Caelum OS 1（天穹）\n");
        printf("系统版本：%s\n", CAELUM_VERSION);
        printf("底层平台：Windows\n");
        printf("代码语言：C++\n");
    } else {
        printf("System Name: Caelum OS 1 (Vault)\n");
        printf("System Version: %s\n", CAELUM_VERSION);
        printf("Base Platform: Windows\n");
        printf("Programming Language: C++\n");
    }
}
void System_Changelog() {
    if (lan == 1) {
        printf("1. 2026/9/26 Caelum OS 1 V_1.0(1000.0000)发布\n");
        printf("2. 2026/9/26 T10002080 功能补丁发布，新增一些指令\n");
        printf("3. 2026/9/27 Caelum OS 1 V_1.1(1100.0000)发布，引入应用Box_Code\n");
        printf("4. 2026/9/27 T11004055 功能补丁发布，加入游戏功能，并引入一款游戏\n");
        printf("5. 2026/9/27 S11004055 安全补丁发布，解决了应用非正常打开导致的卡死问题\n");
        printf("6. 2026/9/28 Caelum OS 1 V_1.2(1200.0000)发布，修改底层代码，引入类似注册表的应用列表\n");
        printf("7. 2026/9/28 T12002819 功能补丁发布，合并打开应用指令与游戏列表指令\n");
        printf("8. 2026/9/28 T12005715 功能补丁发布，新增一款应用\n");
        printf("9. 2026/10/2 Caelum OS 1 V_1.2(1300.0000)发布，底层代码模块化，方便进行自创\n");
        printf("10. 2026/10/2 S13002832 安全补丁发布，用户可新增带空格文件名的应用\n");
        printf("11. 2026/10/2 T13004632 功能补丁发布，新增加密字符串应用\n");
        printf("12. 2026/10/05 Caelum OS 1 V_1.4(1400.0000)发布，新增应用中心store，增加扫描防御\n");
        printf("13. 2026/10/06 T14002912 功能补丁发布，新增待办事项列表功能，修复部分bug\n");
        printf("14. 2016/10/06 T14004812 功能补丁发布，完善待办事项功能\n");
    } else {
        printf("1. 2026/9/26 Caelum OS 1 V_1.0(1000.0000) released\n");
        printf("2. 2026/9/26 T10002080 feature patch released, several new commands added\n");
        printf("3. 2026/9/27 Caelum OS 1 V_1.1(1100.0000) released, introduced app Box_Code\n");
        printf("4. 2026/9/27 T11004055 feature patch released, added game function and one game\n");
        printf("5. 2026/9/27 S11004055 security patch released, fixed freeze caused by abnormal app launching\n");
        printf("6. 2026/9/28 Caelum OS 1 V_1.2(1200.0000) released, modified underlying code, introduced registry‑like app list\n");
        printf("7. 2026/9/28 T12002819 feature patch released, merged launch-app command and game-list command\n");
        printf("8. 2026/9/28 T12005715 Feature Patch released, one new application added\n");
        printf("9. 2026/10/2 Caelum OS 1 V_1.3(1300.0000) released, underlying code modularized for self‑creation\n");
        printf("10. 2026/10/2 S13002832 security patch released, users can add applications with space‑containing filenames\n");
        printf("11. 2026/10/2 T13004632 Function patch released, added encrypted‑string application\n");
        printf("12. 2026/10/05 Caelum OS 1 V_1.4(1400.0000) released, Add App Center(store), add scan defence\n");
        printf("13. 2026/10/06 T14002912 Feature Patch released, added To‑Do List feature, fixed some bugs\n");
        printf("14. 2016/10/06 T14004812 Feature Patch released, improved To‑Do List feature\n");
    }
}
void Store() {
    int sel;
    while (true) {
        system("cls");
        SetConsoleTextAttribute(hConsole, 0x0B);
        if (lan == 1) {
            printf("========== 应用中心 ==========\n");
            printf("本机已安装应用：\n");
            for (int i = 0; i < app_count; i++) {
                printf("[%d] %s\n", i + 1, app_db[i].name_zh.c_str());
            }
            printf("\n请输入应用编号打开详情；输入0退出应用中心\n> ");
        } else {
            printf("========== App Center ==========\n");
            printf("Installed Applications:\n");
            for (int i = 0; i < app_count; i++) {
                printf("[%d] %s\n", i + 1, app_db[i].name_en.c_str());
            }
            printf("\nInput app number for detail; 0 to exit\n> ");
        }
        SetConsoleTextAttribute(hConsole, 0x07);
        if (!read_integer("", sel)) {
            if (cin.eof())
                return;
            SetConsoleTextAttribute(hConsole, 0x0C);
            if (lan == 1)
                printf("请输入有效整数！\n");
            else
                printf("Please enter a valid integer.\n");
            SetConsoleTextAttribute(hConsole, 0x07);
            continue;
        }
        if (sel == 0) {
            system("cls");
            SetConsoleTextAttribute(hConsole, 0x07);
            break;
        }
        if (sel < 1 || sel > app_count) {
            SetConsoleTextAttribute(hConsole, 0x0C);
            if (lan == 1)
                printf("无效编号！\n");
            else
                printf("Invalid index!\n");
            Sleep(1000);
            continue;
        }
        int idx = sel - 1;
        system("cls");
        SetConsoleTextAttribute(hConsole, 0x0E);
        if (lan == 1) {
            printf("====应用详情====\n");
            printf("名称：%s\n", app_db[idx].name_zh.c_str());
            printf("版本：%s\n", app_db[idx].version.c_str());
            printf("分类：%s\n", app_db[idx].category.c_str());
            printf("作者：%s\n", app_db[idx].author.c_str());
            printf("\n按1启动该应用；其他数字返回列表\n> ");
        } else {
            printf("====App Detail====\n");
            printf("Name：%s\n", app_db[idx].name_en.c_str());
            printf("Version：%s\n", app_db[idx].version.c_str());
            printf("Category：%s\n", app_db[idx].category.c_str());
            printf("Author：%s\n", app_db[idx].author.c_str());
            printf("\nPress 1 to launch; other number back\n> ");
        }
        SetConsoleTextAttribute(hConsole, 0x07);
        int op;
        if (!read_integer("", op)) {
            if (cin.eof())
                return;
            SetConsoleTextAttribute(hConsole, 0x0C);
            if (lan == 1)
                printf("请输入有效整数！\n");
            else
                printf("Please enter a valid integer.\n");
            SetConsoleTextAttribute(hConsole, 0x07);
            continue;
        }
        if (op == 1) {
            const string& run_exe =
                lan == 1 ? app_db[idx].bin_zh : app_db[idx].bin_en;
            launch_app(app_db[idx], run_exe);
        }
    }
}
void App() {
    SetConsoleTextAttribute(hConsole, 0x02);
    for (int i = 0; i < app_count; i++) {
        if (lan == 1) {
            printf("%d.%s\n", i + 1, app_db[i].name_zh.c_str());
        } else {
            printf("%d.%s\n", i + 1, app_db[i].name_en.c_str());
        }
    }
    SetConsoleTextAttribute(hConsole, 0x07);
    int n;
    if (!read_integer("> ", n)) {
        if (!cin.eof()) {
            if (lan == 1)
                printf("请输入有效整数！\n");
            else
                printf("Please enter a valid integer.\n");
        }
        return;
    }
    if (n >= 1 && n <= app_count) {
        int idx = n - 1;
        const string& run_exe =
            lan == 1 ? app_db[idx].bin_zh : app_db[idx].bin_en;
        launch_app(app_db[idx], run_exe);
    } else {
        SetConsoleTextAttribute(hConsole, 0x0C);
        if (lan == 1)
            printf("没有这个应用\n");
        else
            printf("Don't have this app\n");
        SetConsoleTextAttribute(hConsole, 0x07);
    }
}
} // namespace cpu

namespace other {
bool BIOS() {
    while (true) {
        printf("please choose language: 1.Chinese  2.English\n");
        int choice;
        if (!read_integer("> ", choice)) {
            if (cin.eof())
                return false;
            printf("Please enter 1 or 2.\n");
            continue;
        }
        lan = choice;
        if (lan == 1) {
            printf("OK,you choose Chinese\n");
            break;
        } else if (lan == 2) {
            printf("OK,you choose English\n");
            break;
        } else
            printf("Don't have this language,please choose again\n");
    }
    printf("Downloading language pack...\n");
    Sleep(1200);
    printf("Language pack download completed.\n");
    return true;
}
bool Begin() {
    {
        json j_color;
        ifstream fin("../system/color.json");
        WORD startup_color = 0x07;
        if (fin.is_open()) {
            try {
                fin >> j_color;
                auto color = j_color.find("console_color");
                if (j_color.is_object() && color != j_color.end() &&
                    color->is_number_integer()) {
                    int value = color->get<int>();
                    if (value >= 0 && value <= 0xFF)
                        startup_color = static_cast<WORD>(value);
                    else
                        cerr << "主题颜色超出范围，使用默认值 / Theme color out of range; using default\n";
                } else {
                    cerr << "主题配置格式无效，使用默认值 / Invalid theme config; using default\n";
                }
            } catch (const json::exception& e) {
                cerr << "主题 JSON 无效，使用默认值 / Invalid theme JSON; using default: "
                     << e.what() << '\n';
            }
        }
        set_full_console_color(startup_color);
    }

    std::random_device rd;
    mt19937 rnd(rd());
    printf("Starting boot screen...\n");
    Sleep(1234);
    if (!BIOS())
        return false;
    scan_app_folder();
    if (lan == 1) {
        printf("正在启动Caelum OS 1，请稍等...\n");
        Sleep(rnd() % 1000 + 1000);
        printf("启动成功，欢迎！\n");
        Sleep(1000);
        system("cls");
    } else {
        printf("Starting Caelum OS 1, please wait...\n");
        Sleep(rnd() % 1000 + 1000);
        printf("Boot completed, Welcome!\n");
        Sleep(1000);
        system("cls");
    }
    SetConsoleTextAttribute(hConsole, 0x0B);
    printf("================================Caelum OS 1================================\n");
    SetConsoleTextAttribute(hConsole, 0x07);
    return true;
}
} // namespace other

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (!other::Begin())
        return 0;
    string line;
    vector<string> args;
    while (getline(cin, line)) {
        if (line.size() > 4096) {
            if (lan == 1)
                printf("命令过长。\n");
            else
                printf("Command is too long.\n");
            continue;
        }
        if (!split_command(line, args)) {
            if (lan == 1)
                printf("引号未闭合。\n");
            else
                printf("Unclosed quote.\n");
            continue;
        }
        if (args.empty())
            continue;

        if (args[0] == "time" && args.size() == 1) {
            cpu::Time();
        } else if (args[0] == "exit" && args.size() == 1) {
            cpu::System_Exit();
        } else if (args[0] == "clean" && args.size() == 1) {
            system("cls");
        } else if (args[0] == "help" && args.size() == 1) {
            cpu::Help();
        } else if (args[0] == "color") {
            cpu::Color(args);
        } else if (args[0] == "theme") {
            cpu::Theme(args);
        } else if (args[0] == "system" && args.size() == 2) {
            if (args[1] == "information") {
                cpu::System_Information();
            } else if (args[1] == "changelog") {
                cpu::System_Changelog();
            } else {
                if (lan == 1)
                    printf("无效的 system 子命令。\n");
                else
                    printf("Invalid system subcommand.\n");
            }
        } else if (args[0] == "app" && args.size() == 1) {
            cpu::App();
        } else if (args[0] == "store" && args.size() == 1) {
            cpu::Store();
        } else {
            SetConsoleTextAttribute(hConsole, 0x0C);
            if (lan == 1) {
                printf("无效的命令！\n");
            } else {
                printf("Invalid command!\n");
            }
            SetConsoleTextAttribute(hConsole, 0x07);
        }
    }
    return 0;
}