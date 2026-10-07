#include <bits/stdc++.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <windows.h>
#define CAELUM_VERSION "V_1.4(1400.4812)"
using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;
const int MAX_APP = 20;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void set_full_console_color(WORD attr)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    COORD start = {0,0};
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

void scan_app_folder() {
    app_count = 0;
    for (auto &entry : fs::directory_iterator("../app")) {
        if (!entry.is_directory())
            continue;
        fs::path jsonpath = entry.path() / "app.json";
        if (app_count >= MAX_APP)
            break;
        ifstream fin(jsonpath);
        if (!fin.is_open())
            continue;
        json j;
        fin >> j;
        AppItem &it = app_db[app_count];
        it.dir_path = entry.path().string();
        it.name_zh = j["name"]["zh-CN"].get<string>();
        it.name_en = j["name"]["en-US"].get<string>();
        it.bin_zh = j["bin"]["zh-CN"].get<string>();
        it.bin_en = j["bin"]["en-US"].get<string>();
        it.version = j["version"].get<string>();
        it.category = j["category"].get<string>();
        it.author = j["author"].get<string>();
        app_count++;
    }
}

string s, a[105];
int lan, n, number_v = 1;

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
void Color() {
    if (number_v < 2) {
        SetConsoleTextAttribute(hConsole, 0x0C);
        if (lan == 1)
            printf("用法：color 0B\n");
        else
            printf("Usage: color 0B\n");
        SetConsoleTextAttribute(hConsole, 0x07);
    } else {
        int c;
        sscanf(a[2].c_str(), "%x", &c);
        SetConsoleTextAttribute(hConsole, (WORD)c);
    }
}

void Theme()
{
    if(number_v < 2)
    {
        SetConsoleTextAttribute(hConsole,0x0C);
        if(lan == 1)
            printf("用法：theme 17 （十六进制颜色码，高位背景，低位前景）\n");
        else
            printf("Usage: theme 17 (hex code, high‑4 background, low‑4 foreground)\n");
        SetConsoleTextAttribute(hConsole,0x07);
        return;
    }
    int c;
    sscanf(a[2].c_str(),"%x",&c);
    WORD attr = (WORD)c;
    set_full_console_color(attr);
    json j_out;
    j_out["console_color"] = (int)attr;
    ofstream fout("../system/color.json");
    if(!fout.is_open())
    {
        SetConsoleTextAttribute(hConsole,0x0C);
        if(lan ==1) printf("保存颜色配置失败！\n");
        else printf("Failed saving color config!\n");
        SetConsoleTextAttribute(hConsole,0x07);
        return;
    }
    fout << j_out.dump(4);
    fout.close();
    SetConsoleTextAttribute(hConsole,0x0A);
    if(lan ==1) printf("主题颜色已经设置并保存！\n");
    else printf("Theme color applied and saved!\n");
    SetConsoleTextAttribute(hConsole,0x07);
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
        scanf("%d", &sel);
        {
            string dummy;
            getline(cin, dummy);
        }
        if (sel == 0){
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
        scanf("%d", &op);
        {
            string dummy;
            getline(cin, dummy);
        }
        if (op == 1) {
            string run_exe;
            if (lan == 1)
                run_exe = app_db[idx].bin_zh;
            else
                run_exe = app_db[idx].bin_en;
            string cmd = "start \"\" \"" + app_db[idx].dir_path + "\\" + run_exe + "\"";
            system(cmd.c_str());
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
    scanf("%d", &n);
    {
        string dummy;
        getline(cin, dummy);
    }
    if (n >= 1 && n <= app_count) {
        int idx = n - 1;
        string run_exe;
        if (lan == 1) {
            run_exe = app_db[idx].bin_zh;
        } else {
            run_exe = app_db[idx].bin_en;
        }
        string cmd = "start \"\" \"" + app_db[idx].dir_path + "\\" + run_exe + "\"";
        system(cmd.c_str());
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
void BIOS() {
    while (true) {
        printf("please choose language: 1.Chinese  2.English\n");
        scanf("%d", &lan);
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
    return;
}
void Begin() {
    //开机读取保存的终端主题颜色
    {
        json j_color;
        ifstream fin("../system/color.json");
        WORD startup_color = 0x07;
        if(fin.is_open())
        {
            fin >> j_color;
            startup_color = (WORD)j_color["console_color"].get<int>();
            fin.close();
        }
        set_full_console_color(startup_color);
    }

    std::random_device rd;
    mt19937 rnd(rd());
    printf("Starting boot screen...\n");
    Sleep(1234);
    BIOS();
    {
        string dummy;
        getline(cin, dummy);
    }
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
}
} // namespace other
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    other::Begin();
    while (1) {
        for (int i = 1; i <= 100; i++)
            a[i].clear();
        getline(cin, s);
        int len = (int)s.size();
        while (len > 0 && s[len - 1] == ' ') {
            len--;
        }
        number_v = 1;
        for (int i = 0; i < len; i++) {
            if (s[i] == ' ')
                number_v++;
            else
                a[number_v] += s[i];
        }
        if (number_v == 1 && a[1].empty())
            continue;
        if (a[1] == "time" && number_v == 1) {
            cpu::Time();
        } else if (a[1] == "exit" && number_v == 1) {
            cpu::System_Exit();
        } else if (a[1] == "clean" && number_v == 1) {
            system("cls");
        } else if (a[1] == "help" && number_v == 1) {
            cpu::Help();
        } else if (a[1] == "color") {
            cpu::Color();
        } else if (a[1] == "theme") {
            cpu::Theme();
        } else if (a[1] == "system") {
            if (a[2] == "information" && number_v == 2) {
                cpu::System_Information();
            } else if (a[2] == "changelog" && number_v == 2) {
                cpu::System_Changelog();
            }
        } else if (a[1] == "app" && number_v == 1) {
            cpu::App();
        } else if (a[1] == "store" && number_v == 1) {
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
    system("pause");
    return 0;
}
