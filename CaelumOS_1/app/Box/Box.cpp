#include <iostream>
#include <cstdio>
#include <algorithm>
#include <random>
#include <windows.h>
#define w_var_num 18
#define wait_time 10
#define MAX_V 1000
using namespace std;

long long x, y, z, n, m, num = 0, v[1001];
string s, a[101], var[1001], w_var[101] = {"input", "output", "set", "if", "else", "for",
                                           "while", "int", "char", "string", "namespace",
                                           "include", "elif", "error", "ifdef", "ifndef",
                                           "line", "progma"
                                          };

namespace other {
	bool find_str(const string s) {
		for (int i = 1; i <= num; i++) {
			if (var[i] == s) {
				return true;
			}
		}
		return false;
	}
	bool find_var(const string s) {
		for (int i = 0; i < w_var_num; i++) {
			if (s == w_var[i]) {
				return true;
			}
		}
		return false;
	}
	int find_index(const string c) {
		for (int i = 1; i <= num; i++) {
			if (c == var[i]) {
				return i;
			}
		}
		return -1;
	}
	bool digit(const string s) {
		for (char c : s) {
			if (!(c >= '0' && c <= '9')) {
				return false;
			}
		}
		return true;
	}
	bool var_num(const string s) {
		if (s.empty() || (s[0] >= '0' && s[0] <= '9')) {
			return false;
		}
		if (other::find_var(s)) {
			return false;
		}
		for (char c : s) {
			if (!(((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')) ||
			        ((c >= 'A' && c <= 'Z') || (c == '_')))) {
				return false;
			}
		}
		return true;
	}
	long long memory_v(int index, int op, long long v_num) {
		if (op == 0) {
			return v[index];
		} else {
			v[index] = v_num;
		}
		return 0;
	}
	string memory_var(int index, int op, string var_name) {
		if (op == 0) {
			return var[index];
		} else {
			var[index] = var_name;
		}
		return " ";
	}
}

namespace cpu {
	const long long ERR_MARK = -9223333333333333333LL;

	long long dfs_expr(const string &str, int l, int r) {
		int x = 0;
		int addsub_pos = 0;
		int muldiv_pos = 0;
		for (int i = l; i <= r; i++) {
			if (str[i] == '(') x++;
			else if (str[i] == ')') x--;
			else if (!x) {
				if (str[i] == '+' || str[i] == '-') {
					addsub_pos = i;
				}
			}
		}
		if (addsub_pos != 0) {
			char op = str[addsub_pos];
			long long left = dfs_expr(str, l, addsub_pos - 1);
			long long right = dfs_expr(str, addsub_pos + 1, r);
			if (left == ERR_MARK || right == ERR_MARK) return ERR_MARK;
			if (op == '+') return left + right;
			else return left - right;
		}
		x = 0;
		for (int i = l; i <= r; i++) {
			if (str[i] == '(') x++;
			else if (str[i] == ')') x--;
			else if (!x) {
				if (str[i] == '*' || str[i] == '/') {
					muldiv_pos = i;
				}
			}
		}
		if (muldiv_pos != 0) {
			char op = str[muldiv_pos];
			long long left = dfs_expr(str, l, muldiv_pos - 1);
			long long right = dfs_expr(str, muldiv_pos + 1, r);
			if (left == ERR_MARK || right == ERR_MARK) return ERR_MARK;
			if (op == '*') {
				return left * right;
			} else {
				if (right == 0) return ERR_MARK;
				return left / right;
			}
		}
		if (str[l] == '(' && str[r] == ')') {
			return dfs_expr(str, l + 1, r - 1);
		}
		string token;
		for (int i = l; i <= r; i++) token += str[i];
		if (other::digit(token)) {
			return stoll(token);
		} else {
			int idx = other::find_index(token);
			if (idx == -1) return ERR_MARK;
			return other::memory_v(idx, 0, 0);
		}
	}

	long long operation(long long x1, long long x2, long long op) {
		if (op == 1) {
			return x1 + x2;
		} else if (op == 2) {
			return x1 - x2;
		} else if (op == 3) {
			return x1 * x2;
		} else if (op == 4) {
			return x1 / x2;
		} else if (op == 5) {
			return x1 % x2;
		}
		return -1;
	}
}

int main() {
	std::random_device rd;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	memset(v, 0, sizeof(v));
	mt19937 rnd(rd());
	printf("This is the app of Box_Code 1.3.You can write code in this app.\n"
	       "You must run this app on Windows, and compile it with C++.\nPlease wait¡­¡­\n");
	Sleep(rnd() % 1501 + 1000);
	printf("OK,now you can write code in Box_Code 1.3.\n");
	while (1) {
		for (int i = 1; i <= 100; i++) a[i].clear();
		getline(cin, s);
		int number_v = 1, len = (int)s.size();
		for (int i = 0; i < len; i++) {
			if (s[i] == ' ') {
				number_v++;
				continue;
			}
			a[number_v] = a[number_v] + s[i];
		}
		int real_nv = number_v;
		int pos_semi = -1;
		for (int i = 1; i <= number_v; i++) {
			if (a[i] == ";") {
				pos_semi = i;
				real_nv = i - 1;
				break;
			}
		}
		if (pos_semi != -1) {
			number_v = real_nv;
		} else {
			if (a[number_v].empty()) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Missing semicolon\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (a[number_v].back() != ';') {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Missing semicolon\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			a[number_v].pop_back();
		}

		if (a[1] == "exit" && number_v == 1) {
			exit(1);
		} else if (a[1] == "calc") {
			string expr;
			for (int i = 2; i <= number_v; i++) {
				expr += a[i];
			}
			long long res = cpu::dfs_expr(expr, 0, (int)expr.size() - 1);
			if (res == cpu::ERR_MARK) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] evaluate expression failed, undefined variable or division by zero\n");
				SetConsoleTextAttribute(hConsole, 0x07);
			} else {
				printf("%lld\n", res);
			}
			continue;
		} else if (a[1] == "set") {
			if (other::find_str(a[2])) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Duplicate variable name\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (!other::var_num(a[2])) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] illegal identifier\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (num == MAX_V) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Insufficient storage for new variable\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (number_v == 2) {
				num++;
				other::memory_var(num, 1, a[2]);
				printf("OK,you create a new variable name:");
				cout << other::memory_var(num, 0, "0") << endl;
				continue;
			}
			if (a[3] != "=" || number_v != 4) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			num++;
			other::memory_var(num, 1, a[2]);
			other::memory_v(num, 1, stoll(a[4]));
			printf("OK,you create a new variable name:");
			cout << a[2] << endl;
			continue;
		} else if (a[1] == "input") {
			if (number_v != 2) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (!other::find_str(a[2])) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Undefined variable\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			printf("Please input:");
			long long new_v;
			scanf("%lld", &new_v);
			while (getchar() != '\n');
			int index = other::find_index(a[2]);
			other::memory_v(index, 1, new_v);
			continue;
		} else if (a[1] == "output") {
			if (number_v != 2) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (!other::find_str(a[2])) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Undefined variable\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			printf("%lld\n", other::memory_v(other::find_index(a[2]), 0, 0));
			continue;
		} else if (a[1] == "assign") {
			if (!other::find_str(a[2])) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Undefined variable\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (a[3] != "=" || number_v != 4) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			int idx = other::find_index(a[2]);
			long long val;
			if (other::digit(a[4])) {
				val = stoll(a[4]);
			} else {
				int idx2 = other::find_index(a[4]);
				val = v[idx2];
			}
			v[idx] = val;
			printf("OK assign done %lld\n", v[idx]);
			continue;
		} else {
			if (number_v != 3) {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error]Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (a[2] == "+") {
				y = 1;
			} else if (a[2] == "-") {
				y = 2;
			} else if (a[2] == "*") {
				y = 3;
			} else if (a[2] == "/") {
				y = 4;
			} else if (a[2] == "%") {
				y = 5;
			} else if (a[2] == "+=") {
				y = 6;
			} else if (a[2] == "-=") {
				y = 7;
			} else if (a[2] == "*=") {
				y = 8;
			} else if (a[2] == "/=") {
				y = 9;
			} else if (a[2] == "%=") {
				y = 10;
			} else {
				SetConsoleTextAttribute(hConsole, 0x0C);
				printf("[Error] Unknown instruction\n");
				SetConsoleTextAttribute(hConsole, 0x07);
				continue;
			}
			if (y <= 5) {
				if (!other::digit(a[1])) {
					int index = other::find_index(a[1]);
					if (index == -1) {
						SetConsoleTextAttribute(hConsole, 0x0C);
						printf("[Error] Undefined variable\n");
						SetConsoleTextAttribute(hConsole, 0x07);
						continue;
					}
					x = other::memory_v(index, 0, 0);
				} else {
					x = stoll(a[1]);
				}
				if (!other::digit(a[3])) {
					int index = other::find_index(a[3]);
					if (index == -1) {
						SetConsoleTextAttribute(hConsole, 0x0C);
						printf("[Error] Undefined variable\n");
						SetConsoleTextAttribute(hConsole, 0x07);
						continue;
					}
					z = other::memory_v(index, 0, 0);
				} else {
					z = stoll(a[3]);
				}
				long long ans = cpu::operation(x, z, y);
				printf("%lld\n", ans);
			} else {
				int index = other::find_index(a[1]);
				if (index == -1) {
					SetConsoleTextAttribute(hConsole, 0x0C);
					printf("[Error] Undefined variable\n");
					SetConsoleTextAttribute(hConsole, 0x07);
					continue;
				}
				x = v[index];
				if (!other::digit(a[3])) {
					int index = other::find_index(a[3]);
					if (index == -1) {
						SetConsoleTextAttribute(hConsole, 0x0C);
						printf("[Error] Undefined variable\n");
						SetConsoleTextAttribute(hConsole, 0x07);
						continue;
					}
					z = other::memory_v(index, 0, 0);
				} else {
					z = stoll(a[3]);
				}
				long long ans = cpu::operation(x, z, y - 5);
				index = other::find_index(a[1]);
				other::memory_v(index, 1, ans);
				printf("%lld\n", v[index]);
			}
		}
	}
	return 0;
}
