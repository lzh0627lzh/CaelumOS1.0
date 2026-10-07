#include <bits/stdc++.h>
#include <windows.h>
#define LEN 100000001
#define P 100000000001LL
#define L 32

using namespace std;
long long a[256] = {-412,73,-9,510821,-6624,30,881,-22751,9470,-33,726192,-5,163,-8042,27480,-11,62,90341,-775,48,-36102,591,-82,3706,-149273,6,2195,-407,83620,-18,544,71306,-291,8125,-630,47,-95183,260,-74,13092,-5881,316,-7,9641,-28046,152,-833,60714,-21,490,-7012,845,-366,10973,-52,781,-43097,2056,-148,350,-6194,9273,-30,114,-85261,408,-279,6740,-13508,22,-916,53817,-42,860,-1703,31945,-8,702,-66430,191,-234,4106,-11072,54,-509,87362,-16,337,-9451,2647,-710,180,-38205,741,-26,9083,-47614,39,-887,56021,-190,643,-3278,17492,-57,811,-76048,243,-123,4670,-20186,68,-441,79253,-310,526,-1574,34618,-13,930,-54927,117,-602,8409,-37152,45,-758,61740,-225,384,-8106,28371,-4,766,-68319,158,-107,5043,-24673,82,-354,91627,-173,672,-4159,21045,-63,894,-73401,271,-142,4368,-18520,31,-586,57934,-240,729,-2963,12758,-48,857,-64172,104,-801,7451,-34081,61,-923,80649,-116,489,-7722,36910,-27,975,-51306,236,-169,4027,-22847,77,-473,69881,-208,550,-1319,29744,-9,832,-78653,147,-131,4495,-16735,53,-649,95018,-186,704,-3542,19367,-72,888,-62740,288,-257,5216,-27194,41,-845,71093,-154,621,-4607,14826,-36,919,-56811,129,-714,8630,-31758,67,-406,65472,-231,583,-9731,32057,-22,750,-69428,176,-119,4782,-21360,89,-537,82741,-197,665,-2480,25103,-59,941,-74836,214,-145,3891,-17942,46,-770,59628,-263,784};
long long g,salt = 519728406LL;
int _l;
unsigned char c[LEN],s[L + 2],m[L + 2][L + 2];

long long calc1(long long x,long long y){
    long long b = a[((x % 256) + 1) % 256];
    long long d = (b ^ a[((b % 256) * (b % 256) % 256 + 1) % 256]);
    long long p = (((d ^ g) * (d ^ y) % P) ^ ((g ^ x) % P)) % P;
    long long q = (((d ^ p) % P) ^ ((g ^ y) % P)) % P;
    long long f = ((((b ^ d) % P) * ((g ^ d) % P) % P + ((b ^ g) % P) * ((b ^ p) % P) % P) % P + ((b ^ q) % P) * ((b ^ g) % P) % P) % P;
    d = (d % P + P) % P;
    if(d == 0) d = 1;
    long long res = ((((f % d) ^ g) % P) ^ q) % P;
    return (res % P + P) % P;
}
void calc2(int x,long long xm,long long ym){
    long long b = a[((((x ^ xm) % P) % 256) + 1) % 256];
    const int ROUND = 6;
    for(int i = 1;i <= ROUND;i++){
        for(int j = 1;j <= L;j++){
            long long f = (((((xm ^ b) % P) * ((g ^ ym) % P)) % P) ^ ((b ^ j) % P)) % P;
            long long _y = (f % L + L) % L + 1;
            m[x][j] = calc1(f,_y);
            m[x][_y] = calc1(((b ^ _y) % P),((b ^ f) % P));
            swap(m[x][j],m[x][_y]);
        }
    }
}
void calc3(long long x){
    long long b = a[((x % 256) + 1) % 256];
    long long f = (((((x ^ g) % P) ^ ((b ^ g) % P)) % P) ^ ((((x ^ b) % P) ^ ((b ^ g) % P)) % P)) % P;
    for(int i = 1;i <= L;i++){
        for(int j = 1;j <= L;j++){
            long long t = (((f ^ b) % P) ^ ((i ^ j) % P)) % P;
            long long _x = ((t ^ a[((b % 256) + 1) % 256]) % L + L) % L + 1;
            long long _y = ((t ^ f) % L + L) % L + 1;
            m[_x][_y] = calc1(_x,_y);
            m[i][j] = calc1(t,((_x ^ _y)) % P);
            swap(m[i][j],m[_x][_y]);
        }
    }
}

void jm(){
    scanf("%s",c + 1);
    _l = strlen((const char *)(c + 1));
    g = 0;
    const int R = 6;
    for(int tp = 1;tp <= R;tp++){
		for(int i = 1;i <= _l;i++){
		    g += c[i];
		    g %= P;
			long long t = (c[i] ^ salt) % P;
		    g = g ^ (t * t % P) % P;
	        g = (g % P + P) % P;
		}
	}
	if(_l > 16){
		for(int i = 1;i <= _l;i++){
			if(i == 16){
				s[16] += c[i];
			}else{
				s[i % L] += c[i];
			}
		}
	}else{
		for(int i = 1;i <= 16;i++){
			if(i % _l == 0){
				s[i] = c[_l];
			}else{
				s[i] = c[i % _l];
			}
		}
	}
    for(int i = 1;i <= L;i++){
        for(int j = 1;j <= L;j++){
            m[i][j] = s[j];
        }
    }
    for(int tp = 1;tp <= R;tp++){
        for(int i = 1;i <= L;i++){
            for(int j = 1;j <= L;j++){
                long long b = a[((((i ^ j) % P) % 256) + 1) % 256];
                long long f = (((m[i][j] ^ b) % P) * ((m[i][j] ^ g) % P)) % P;
                m[i][j] = calc1(f,m[i][j]);
            }
            long long b = a[((((tp ^ i) % P) % 256) + 1) % 256];
            long long f = (((g ^ b) % P) * ((b ^ s[i]) % P)) % P;
            long long t = (((f ^ g) % P) * ((b ^ g) % P)) % P;
            calc2(i,f,t);
        }
        long long b = a[((tp % 256) + 1) % 256];
        long long f = (((g ^ b) % P) ^ ((b ^ s[tp]) % P)) % P;
        calc3(f);
    }
    for(int i = 1;i <= L;i++){
        long long b = a[((i % 256) + 1) % 256];
        long long f = ((b ^ g) % L + L) % L + 1;
        printf("%02x",(unsigned char)m[i][f]);
    }
    printf("\n");
    return;
}

int main(){
	int num;
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	printf("请输入加密字符串个数：");
	scanf("%d",&num);
	for(int i = 1;i <= num;i++)
		jm();
	system("pause");
	return 0;
}
