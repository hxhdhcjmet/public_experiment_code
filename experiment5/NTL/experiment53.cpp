//Pollard’s Rho算法求解离散对数问题(DLP)
#include<iostream>
#include<NTL/ZZ.h>
#include<NTL/mat_ZZ.h>
#include<NTL/vec_ZZ.h>
#include<vector>
#include<NTL/ZZ_p.h>
#include<chrono>
using namespace std;
using namespace NTL;
using namespace chrono;
bool solve_congruence(vector<ZZ>& X,const ZZ&a,const ZZ& b,const ZZ& n ){
    //求解同余方程ax = b mod n
    ZZ g = GCD(a,n);
    if(b%g!=0){
        X.push_back(ZZ(-1));
        return false;//如果gcd(a,n) 不整除 b则无解
    }
    if(g == 1){
        //唯一解
        ZZ x = (InvMod(a,n)*b) % n;
        X.push_back(x);
        return true;
    }
    ZZ d = n/g;
    ZZ a_1 = a/g;
    ZZ b_1 = b/g;
    ZZ n_1 = d;
    //转换为求a_1*x = b_1 mod n-1
    ZZ x_1 = (InvMod(a_1,n_1)*b_1)%n_1;
    //还原为模n的解
    for(ZZ i(0);i<d;i++){
        ZZ x = (x_1+i*d)%n;
        X.push_back(x);
    }
    return true;
}

struct Element{
    //满足 v = g^ah^b
    ZZ_p v;
    ZZ a,b;
};

Element f(const Element& x,const ZZ_p& g,const ZZ_p& h,const ZZ & n){
    Element res;//创建迭代函数
    ZZ val = rep(x.v);
    switch(conv<long>(val%3)){
        case 0:{//乘以g,g^ah^b*g = g^a+1h^b
            res.v = x.v*g;
            res.a = (x.a+1) % n;
            res.b = x.b % n;
            break;
        }
        case 1:{//乘以h,g^ah^b*h = g^ah^b=1
            res.v = x.v*h;
            res.a = x.a%n;
            res.b = (x.b+1)%n;
            break;
        }
        case 2:{//平方 (g^ah^b)^2 = g^2ah^2b
            res.v = x.v*x.v;
            res.a = (x.a*2)%n;
            res.b = (x.b*2)%n;
            break;
        }
    }
    return res;
}
ZZ pollards_rho(const ZZ& g,const ZZ& h,const ZZ& p){
    //求同余方程g^x = h mod p
    ZZ n(p-1);//阶为n = p-1
    ZZ_p::init(p);
    ZZ_p g_p = conv<ZZ_p>(g);
    ZZ_p h_p = conv<ZZ_p>(h);//转换到有限域上省
    
    //尝试碰撞
    while(true){
        //龟兔赛跑，龟走一步，兔走两步
        Element tortoise,hare;
        tortoise.v = 1;tortoise.a = 0;tortoise.b = 0;//初始化tortoise、hare
        hare = tortoise;
        
        //迭代,tortoise一步，hare两步
        do{
            tortoise = f(tortoise,g_p,h_p,n);
            hare = f(f(hare,g_p,h_p,n),g_p,h_p,n);
        }while(tortoise.v != hare.v);//直到碰撞才结束

        //碰撞,g^tortoise.ah^tortoise.b = g^hare.ah^hare.b
        //创建同余方程 bx = a mod n
        ZZ b = (tortoise.b - hare.b)%n;
        ZZ a = (hare.a - tortoise.a)%n;
        a = (a+n)%n;b = (b+n)%n;//确保非负
        if(b == 0) continue;//无效碰撞

        //求解同余方程
        vector<ZZ> X;
        if(solve_congruence(X,b,a,n)){
            for (const ZZ& x : X){
                if(PowerMod(g,x,p) == h) return x;//可能解中验证一下，确实满足则返回结果
            }
        }
    }
}
int main(){
    vector<ZZ> P = {ZZ(768075001),ZZ(866704609),ZZ(916636177),ZZ(652580214507899),ZZ(827370290159669027)};
    vector<ZZ> G = {ZZ(17),ZZ(53),ZZ(5),ZZ(2),ZZ(2)};
    vector<ZZ> H = {ZZ(750713277),ZZ(210331466),ZZ(481985755),ZZ(77240405458667),ZZ(282875805618794488)};
    for(size_t i = 0;i<100;i++){
        cout<<"=";
    }
    cout<<"totally five piece of test data"<<endl;
    cout<<"using pollards_rho algorithm"<<endl;
    for(size_t i = 0;i<50;i++){
        cout<<"=";
    }
    for(size_t i = 0;i<P.size();i++){
        cout<<endl;
        ZZ p(P[i]),g(G[i]),h(H[i]);
        cout<<"begin to solve the number "<<i+1 <<" function..."<<endl;
        auto start = high_resolution_clock::now();
        ZZ result = pollards_rho(g,h,p);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end-start).count();
        cout<<"the solution of number "<< i <<" function: "<<g<<"^x = "<<h<<" mod "<<p<<" is "
        <<result<<endl;
        cout<<"using time:"<<duration<<" ms"<<endl;
    }
    return 0;
    
}