#include <iostream>
#include "numbers.dat"
#include <vector>

const size_t array_prime_size = 100000;

void Is_Prime(std::vector<int>& prime, const size_t size);
void Search_index(int left, int right, int& ind_l, int& ind_r);

int main(int argc, const char* argv[])
{
    if (argc == 1 || argc % 2 != 1){
        return -1;
    }
    std::vector<int> prime(array_prime_size, 1);
    Is_Prime(prime, array_prime_size);
    for(int i = 1; i < argc; i += 2)
    {
        int left = std::atoi(argv[i]);
        int right = std::atoi(argv[i + 1]);
        if (left > right)
        {
            std::cout<<0<<std::endl;
            continue;
        }
        int ind_l = -1;
        int ind_r = -1;
        Search_index(left, right, ind_l, ind_r);
        if ((ind_l == -1) || (ind_r == -1))
        {
            std::cout << 0 << std::endl;
            continue;
        }
        int count = 0;
        for(int ind = ind_l; ind <=ind_r; ind ++)
        {
            count += prime[Data[ind]];
        }
        std::cout << count << std::endl;
    }
    return 0;
}
 

void Search_index(int left, int right, int& ind_l, int& ind_r)
{
    int i = 0;
    for (; (Data[i] < left) && (i < Size); i++){}
    if (Data[i] == left) {
        ind_l = i;
    }
    for (; (Data[i] < right) && (i < Size); i++){}
    if (Data[i] == right){
        ind_r = i;
    }
}

void Is_Prime(std::vector<int>& prime, const size_t size)
{
    prime[1] = 0;
    prime[0] = 0;
    for (int i = 2; i*i <= size; i++)
    {
        for(int k = 2; k*i <= size;  k++){
            prime[k*i] = 0;
        }
    }
}
