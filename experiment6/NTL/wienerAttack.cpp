// wiener连分数攻击RSA(在d较小时)
#include<iostream>
#include<NTL/ZZ.h>
#include<chrono>
#include<string>
#include<vector>
using namespace std;
using namespace NTL;
using namespace chrono;
const size_t testtimes = 3;//定义每组测试次数
string generate_sep(char sep = '=',int length = 80){
return string(length,sep);
}

ZZ lowsqrt(const ZZ& n){
    //计算n根号的下取整
    if(n<0) return ZZ(0);
    ZZ high(1),low(0);
    while(high*high<=n) high*=2;//high乘以2递增
    //二分查找获得最终结果
    while(high - low > 1){
        ZZ mid = (high+low)/2;//high+low平均值
        if (mid*mid<=n){
            low = mid;
        
        }
        else{
            high = mid;
        }
    }
    return low;
}


bool isPerfectSquare(const ZZ& n,ZZ &root){
    //判断n是不是完全平凡数
    if(n<0)return false;
    root = lowsqrt(n);
    return (root*root == n);
}

//计算连分数展开的每一项
vector<pair<ZZ,ZZ>> continued_fractor(const ZZ & e,const ZZ & N){
    //连分数递推公式
    /*
    p_k = a_kp_{k-1} + p_{k-2}
    q_k = a_kq_{k-1} + q_{k-2}
    */
   ZZ a = e,b = N;
   vector<ZZ> partials;
   while(b!=0){
    ZZ q = a/b;
    partials.push_back(q);
    ZZ r = a-q*b; //a = qb+r
    a = b;
    b = r;
}
// 计算每一项的p_i,q_i
    vector<pair<ZZ,ZZ>> conv;
    ZZ p_prev2(0),p_prev1(1),q_prev2(1),q_prev1(0);//p_-2 = 0,p_ -1 = 1;q_-2 = 1,q_-1 = 0;
    for (size_t i =0;i<partials.size();i++){
        //递推公式计算每一项
        ZZ a_i = partials[i];
        ZZ p = a_i*p_prev1+p_prev2;
        ZZ q = a_i*q_prev1+q_prev2;
        //结果存入候选项数组
        conv.emplace_back(p,q);
        //递推
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return conv;
}
vector<ZZ> WienerAttack(const ZZ& e,const ZZ &N){
    //连分数展开攻击
    auto prob_k_d = continued_fractor(e,N);
    for(auto &prob : prob_k_d){
        //取每一项可能项，分别为k,d.由ed = 1 mod varphi(N)->ed = k*phi+1
        /*计算 是否有k | ed -1,否则否决，是则->
          令phi = (ed-1)/k = (p-1)(q-1) = pq-(p+q)+1 = N-(p+q)+1
          令s = N -phi +1 = N-(N-(p+q)+1)+1 = N -N +(p+q) -1 +1 = p+q
          令D = s^2-4N = (p+q)^2 - 4pq = (p-q)^2,
          (s+sqrt(D))/2 = (p+q+p-q)/2 = p
          (s-sqrt(D))/2 =(p+q -p+q)/2 = q
        */

        ZZ k = prob.first;
        ZZ d = prob.second;
        if (k == 0) continue;

        ZZ ed_sub1 = e*d - 1;
        if(ed_sub1%k !=0) continue;//ed-1不是k的倍数,排除

        ZZ phi = ed_sub1/k;
        if(phi<0) continue;

        ZZ s = N-phi+1;
        ZZ D = s*s - 4*N;
        ZZ sqrtD;
        if(!isPerfectSquare(D,sqrtD)) continue;//不是完全平方数则跳过

        ZZ p((s+sqrtD)/2);
        ZZ q((s-sqrtD)/2);

        //最后验证
        if(p<=1||q<=1) continue;
        if(p*q == N){
            cout<<"Successfully find d!"<<endl;
            vector<ZZ> result = {d,k,p,q};
            return result;
        }
    }
     cout<<"Failed!"<<endl;
    return {};
}

//生成小的d才有可能被攻击
vector<ZZ> generate_vulnerable_args(long bits){
    ZZ p,q;
    GenPrime(p,bits);
    do{GenPrime(q,bits);}while(p == q);
    ZZ N = p*q;
    ZZ phi = (p-1)*(q-1);
    
    ZZ n_sqrt = lowsqrt(N);
    ZZ n_4th = lowsqrt(n_sqrt);

    ZZ limit = n_4th /3;//设置范围
    if(limit<5) limit = ZZ(5);

    ZZ d;//设置d在[2,limit]范围内
    while(true){
        d = RandomBnd(limit-2);
        d = 2*d+3;
        if(GCD(d,phi) == 1) break;
    }
    ZZ e = InvMod(d,phi);
    return {e,d,N,p,q};

}


void authorize(const ZZ&d,const ZZ&d_attack){
    if(d == d_attack){
        cout<<":),wiener attack success!"<<endl;
        return;
    }
    cout<<":(,wiener attack fail!"<<endl;
    return;
}

int main(){
     //设置随机数种子
    ZZ seed;
    time_t t;
    time(&t);
    seed = t;
    SetSeed(seed);
    
    cout<<"begin to test attack..."<<endl;
    vector<long> bits = {512,1024};//p,q的大小，p,q大的一般d也大,测试时先用小的
    for(long bit : bits){
        cout<<"size of N:"<<bit<<endl;
        for(size_t i = 0;i<testtimes;i++){
            vector<ZZ> args = generate_vulnerable_args(bit);
            cout<<generate_sep()<<endl;
            cout<<"e:"<<args[0]<<endl;
            cout<<"d:"<<args[1]<<endl;
            cout<<"n:"<<args[2]<<endl;
            cout<<"p:"<<args[3]<<endl;
            cout<<"q:"<<args[4]<<endl;
            cout<<generate_sep()<<endl;

            auto start = high_resolution_clock::now();   
            vector<ZZ> result = WienerAttack(args[0],args[2]);
            auto end = high_resolution_clock::now();//时间统计
            auto us = duration_cast<microseconds>(end-start).count();
            if(result.empty()) continue;
            authorize(args[1],result[0]);
            cout<<"wiener attack using time:"<<us<<"μs"<<endl;
        }
    
    }
    return 0;

}

