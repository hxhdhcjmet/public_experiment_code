test = [4279209601,11626953439,192311489255622,16095650737563753533,45808220048235498832399142287,149735462706744928439306661202330062437,548025133786371639137565044874029366557]
from math import sqrt
from Crypto.Util.number import *
def Trial_Division(n,fac,isprime=0):
    while(isprime == 0):
        for i in range(2,int(sqrt(n))+1):
            isprime = 1
            if n % i == 0:
                fac.append(i)
                n //= i
                isprime = 0
        if isprime:
            fac.append(n)
    return fac
def isprime(n, k=10):
    if n < 2:
        return False
    # 小质数筛
    small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31]
    if n in small_primes:
        return True
    for p in small_primes:
        if n % p == 0:
            return False

    # 写成 n-1 = 2^r * d
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    # 进行 k 轮随机测试
    for _ in range(k):
        a = randrange(2, n - 1)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True
def pollard_rho(n):
    if n % 2 == 0:
        return 2
    c = randrange(1, n - 1)
    g = lambda x: (x*x + c) % n
    x, y, d = 2, 2, 1
    while d == 1:
        x = g(x)
        y = g(g(y))
        d = gcd(abs(x - y), n)
    return d

def rho(n, fac):
    if n == 1:
        return fac
    if isprime(n):
        fac.append(n)
        return fac

    d = pollard_rho(n)
    fac = rho(d, fac)
    fac = rho(n // d, fac)
    return fac
def Pollard_p_1(n):
    B = 2
    x = 2
    while True:
        x = pow(x, B, n)
        p = gcd(x-1, n)
        if p not in [1,n]:
            return p
        B += 1
def Pollard(n,fac):
    if n == 1:
        return fac
    if isprime(n):
        fac.append(n)
        return fac

    d = Pollard_p_1(n)

    fac = Pollard(d, fac)
    fac = Pollard(n // d, fac)
    return fac
print("Trial_Division")
for i in test[:2]:
    fac = Trial_Division(i,[])
    expr = ''
    for j in fac:
        expr += str(j)
        if j != fac[-1]:
            expr += '*'
    print(f"factor {i} = " + expr)

print("pollard_rho")
for i in test[2:5]:
    fac = rho(i,[])
    expr = ''
    for j in fac:
        expr += str(j)
        if j != fac[-1]:
            expr += '*'
    print(f"factor {i} = " + expr)

print("pollard_p-1")
for i in test[5:7]:
    fac = Pollard(i,[])
    expr = ''
    for j in fac:
        expr += str(j)
        if j != fac[-1]:
            expr += '*'
    print(f"factor {i} = " + expr)
