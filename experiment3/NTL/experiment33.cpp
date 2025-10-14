#include<iostream>
#include<NTL/ZZ.h>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
using namespace NTL;
vector<ZZ> get_sd(ZZ n){// 获取奇素数n的n-1=2^s*d中的s,d 
    ZZ s(0),d(0);
    n=n-1;
    while(n%2==0){
        n=n/2;
        s++;
    }
    d=n;
    return {s,d};
}

ZZ power(const ZZ &a,const ZZ &exponent){
    //计算a^d
    ZZ result(1);
    for(ZZ i(0);i<exponent;i++){
        result*=a;
    }
    return result;
}

bool miller_rabin_test(ZZ n){
    if (n<=1) return false;
    if (n==2) return true;
    if (n%2==0) return false;
    //初步检测
    vector<ZZ> sd=get_sd(n);
    ZZ s(sd[0]),d(sd[1]);
    vector<ZZ> base={
        ZZ(2),ZZ(3),ZZ(5),ZZ(7),
        ZZ(11),ZZ(13),ZZ(17),ZZ(19),ZZ(23)
    };//创建基础a
    for (ZZ a : base){
        if (a>=n) continue;

        bool pass=false;//是否通过金策的标志
        //计算a^d
        ZZ r1;
        PowerMod(r1,a,d,n);
        if(r1==1){
            pass=true;
            continue;
        }
        for (ZZ r(0);r<s;r++){
            ZZ r2;
            ZZ exponent=d*power(ZZ(2),r);
            PowerMod(r2,a,exponent,n);
            if(r2==n-1){
                pass=true;
                break;//只要有一个符合同余n-1即同余1则通过检测
            }
        }
        if (!pass) return false;//当前的基不符合要求，则n为合数
    }
    return true;
}

int main(){
      vector<bool> is_prime;
    ifstream file("附件.txt");
    string line;
    while(getline(file,line)){
        if (line.empty()) continue;
        ZZ n(INIT_VAL,line.c_str());//ZZ 类型通过str型初始化
        bool isprime=miller_rabin_test(n);
        if(isprime){
            is_prime.push_back(true);
        }else{
            is_prime.push_back(false);
        }

    }
    file.close();
    for(bool v : is_prime){
        cout<<v<<" ";
    }
    cout<<endl;
  
    return 0;
}
