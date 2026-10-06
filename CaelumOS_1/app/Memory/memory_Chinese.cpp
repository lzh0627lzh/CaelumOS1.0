#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

int a[6],ans = 0;

int main() {
	std::random_device rd;
	mt19937 rnd(rd());
	for (int i = 1; i <= 5; i++) {
		printf("================== Round %d ==================\n",i);
		for(int j = 1;j <= i;j++){
			a[j] = rnd() % 100 + 1;
			printf("%d ",a[j]);
		}
		Sleep((300 + i * 50) * i);
		if(i == 1){
			Sleep(200);
		}
		system("cls");
		printf("================== Round %d ==================\n",i);
		bool m = true;
		for(int j = 1;j <= i;j++){
			int x;
			scanf("%d",&x);
			if(x != a[j]){
				m = false;
			}
		}
		if(m){
			printf("恭喜成功！\n");
			ans++;
		}else{
			printf("继续加油！\n");
		}
		Sleep(1000);
		system("cls");
	}
	printf("共5关，%d关正确，%d关错误\n",ans,5 - ans);
	system("pause");
	return 0;
}
