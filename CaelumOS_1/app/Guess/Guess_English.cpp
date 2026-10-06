#include <bits/stdc++.h>
using namespace std;
long long on, down, m = 1;
int main() {
	std::random_device rd;
	mt19937 rnd(rd());
	printf("Please enter lower bound and upper bound: ");
	scanf("%lld%lld", &down, &on); 
	long long n = rnd() % (on - down + 1) + down;
	long long in;
	while(true) {
		scanf("%lld", &in);
		if(in == n) break;
		if(in > n)
			printf("Too large\n");
		else
			printf("Too small\n");
		m++;
	}
	printf("Congratulations! You got it!!!\n");
	printf("Total guess count: %lld\n", m);
	system("pause");
	return 0;
}
