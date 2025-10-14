#include<iostream>
#include<NTL/ZZ.h>
#include<vector>
#include<fstream>
#include<string>
using namespace std;
using namespace NTL;
vector<ZZ> reducedResiSys(const ZZ& n){
    // 获得n的一个既约剩余系
    vector<ZZ> result;
    ZZ i(1);
    ZZ g;
    while(i<n){
        GCD(g,i,n);//计算g=(i,n)
        if (g==1){
            result.push_back(i);
        }
        i++;
    }
    return result;
}

vector<ZZ> partReduceResi(const ZZ& n,ZZ k){
    //选取k个与n互素的数，因为选取既约剩余系对大整数n较麻烦,同时由于卡迈克尔数的存在，即使即约剩余系全部符合也不能说明是素数
    vector<ZZ> result;
    k=n>k?k:n;

    ZZ i(1);
    ZZ g;
    while(i<n&&i<k){
        GCD(g,i,n);
        if (g==1){
            result.push_back(i);
        }
        i++;
    }
    return result;
}

bool fermat_test(const ZZ&n){
    //fermat检测
    //初步排除明显非素数
    if(n==2) return true;//2为唯一偶素数
    if(n<=1 || n%2==0) return false;//小于等于一,偶数认为不是素数

    ZZ k(10); 
    vector<ZZ> Zn=partReduceResi(n,k);//获取既约剩余
    
    for ( ZZ i : Zn){
        ZZ r;
        PowerMod(r,i,n-1,n);//i^n-1 mod n结果放在r
        if (r!=1){
            return false;//不满足fermat定理，一定不是素数
        }
    }
    return true;//可能是素数
}
int main(){
    vector<bool> is_prime;
    ifstream file("附件.txt");
    string line;
    while(getline(file,line)){
        if (line.empty()) continue;
        ZZ n(INIT_VAL,line.c_str());//ZZ 类型通过str型初始化
        bool isprime=fermat_test(n);
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


