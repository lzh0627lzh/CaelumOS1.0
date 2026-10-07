#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
long long on, down, m = 1;
int main() {
	std::random_device rd;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	mt19937 rnd(rd());
	printf("请输入下限和上限：");
	scanf("%lld%lld", &down, &on); 
	long long n = rnd() % (on - down + 1) + down;
	long long in;
	while(true) {
		scanf("%lld", &in);
		if(in == n) break;
		if(in > n)
			printf("大了\n");
		else
			printf("小了\n");
		m++;
	}
	printf("恭喜成功！！！\n");
	printf("总共猜测次数：%lld\n", m);
	system("pause");
	return 0;
}
