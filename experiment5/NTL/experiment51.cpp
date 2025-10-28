#include<iostream>
#include<NTL/ZZ.h>
#include<chrono>
using namespace std;
using namespace NTL;
ZZ orderMod(ZZ& g,ZZ& p){
    //获取g模p的阶,p为素数
    ZZ phi = p-1;
    for (ZZ i(1);i < phi;i++){
        ZZ result = PowerMod(g,i,p);
        if(result == 1) return i;  //最小i即为阶
    }
    return phi;//否则g为原根
}



ZZ discrete_log_sol(const ZZ& g,const ZZ& h,const ZZ& p){
    //求解离散对数,g^a = h mod p,求其中的a(其中默认g是模p的原根，题目数据符合)
    //首先获取g的阶
    ZZ order = p-1;
    for(ZZ a(1);a<order;a++){
        ZZ result = PowerMod(g,a,p);
        if(result == h) return a;
    }
    return ZZ(-1);//没有找到离散对数的解
}
int main(){
    cout<<"begin to solve..."<<endl;
    auto start = chrono::high_resolution_clock::now();
    ZZ g(17),h( 750713277),p(768075001);
    ZZ a = discrete_log_sol(g,h,p);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(end-start).count();
    cout<<"discrete log solution is "<<a<<endl;
    cout<<"using time:"<<duration<<"min"<<endl;
    return 0;
}