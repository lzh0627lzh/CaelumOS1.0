#include <windows.h>
#include <bits/stdc++.h>

using namespace std;

int main() {
	std::random_device rd;
	mt19937 rnd(rd());
	while(true){
		int a = rnd() % 2 + 1;
		if(a == 1){
			a += 1;
		}
	}
	return 0;
}
