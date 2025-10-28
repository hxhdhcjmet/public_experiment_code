#include<iostream>
#include<NTL/ZZ.h>
#include<map>
#include<NTL/ZZ_p.h>
#include<chrono>
#include<vector>
using namespace NTL;
using namespace std;
using namespace chrono;
ZZ BSGS(const ZZ & g,const ZZ& h,const ZZ& p){ 
    //baby-step-giant-step,大步小步法
    ZZ m = SqrRoot(p)+1;//M,阶已知默认为p-1
    //g^x = h mod p,x = i*n-j
    map<ZZ,ZZ> table;
    ZZ curr = h%p;//计算hg^j,当j=0时,hg^0  = h
    for(ZZ j(0);j<m;j++){
        if(table.find(curr)==table.end()){
            table[curr] = j;//只放最小值，防hg^j = hg^k
        }
        curr = (curr*g)% p;
    }
    ZZ gm = PowerMod(g,m,p);//计算大步g^m
    ZZ current = ZZ(1); //g^im,当i=0时，g^0m = 1
    for(ZZ i(0);i<m;i++){
        auto it = table.find(current);
        if(it != table.end()){
            //找到了
            ZZ j = it->second;
            ZZ x = i*m-j;//找到解了
            return x;
        }
        current = (current*gm)%p;
    }
    return ZZ(-1);//没找到解，返回-1
}
int main(){
    //存放测试数据
    vector<ZZ> P = {ZZ(768075001),ZZ(866704609),ZZ(916636177),ZZ(652580214507899),ZZ(827370290159669027)};
    vector<ZZ> G = {ZZ(17),ZZ(53),ZZ(5),ZZ(2),ZZ(2)};
    vector<ZZ> H = {ZZ(750713277),ZZ(210331466),ZZ(481985755),ZZ(77240405458667),ZZ(282875805618794488)};
    cout<<"totally five piece of test data"<<endl;
    for(size_t i = 0;i<P.size();i++){
        cout<<endl;
        ZZ p(P[i]),g(G[i]),h(H[i]);
        cout<<"begin to solve the number "<<i+1 <<" function..."<<endl;
        auto start = high_resolution_clock::now();
        ZZ result = BSGS(g,h,p);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end-start).count();
        cout<<"the solution of number "<< i <<" function: "<<g<<"^x = "<<h<<" mod "<<p<<" is "
        <<result<<endl;
        cout<<"using time:"<<duration<<" ms"<<endl;
        
    }
    return 0;
}