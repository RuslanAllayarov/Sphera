#include <iostream>
#include <fstream>
#include <algorithm>  
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

void Separation(const char* name, uint64_t kol, uint64_t N, bool& dataReady, bool& complete, mutex& mutex_, condition_variable& ready)
{
    unique_lock<std::mutex> lock(mutex_);
    uint64_t tmp;
    uint64_t count_sorted = N;
    ifstream f;
    ofstream f1, f2;
    bool is_not_open = false;
    while (count_sorted < kol) 
    {
        while (!dataReady)
	    ready.wait(lock);
        if (complete)
	    break;
	f.open(name, ios::binary);
	f1.open("sort_left", ios::binary);
	f2.open("sort_right", ios::binary);
	if (!f1.is_open() || !f2.is_open() || !f.is_open())
	{
	    cout << "Error at opening file in Separation" << endl;
	    is_not_open = true;
	    break; 
	}
	if (!f.eof())  f.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
	while (!f.eof()) 
	{
	    for (uint64_t i = 0; i < count_sorted && !f.eof(); ++i) 
	    {
	        f1.write(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
		f.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
	    }
	    for (uint64_t j = 0; j < count_sorted && !f.eof(); ++j) 
	    {
		f2.write(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
		f.read(reinterpret_cast<char*>(&tmp), sizeof(uint64_t));
	    }
	}
	f.close();
        f1.close();
	f2.close();
	dataReady = false;
	count_sorted *= 2;
	ready.notify_one();
    }
    if(!is_not_open)
    {
	f2.close();
	f1.close();
	f.close();
    }
}


void Merge(const char* name, uint64_t kol, uint64_t N, bool& dataReady, bool& complete, mutex& mutex_, condition_variable& ready)
{
    unique_lock<std::mutex> lock(mutex_);
    uint64_t left, right; // элементы сливаемых массивов
    uint64_t count_sorted = N;
    ifstream f1, f2;
    ofstream f;
    bool is_not_open = false;
    while (count_sorted < kol) 
    {
	while (dataReady)
	    ready.wait(lock);
	f.open(name, ios::binary);
	f1.open("sort_left", ios::binary);
	f2.open("sort_right", ios::binary);
	if (!f1.is_open() || !f2.is_open() || !f.is_open())
	{
	    cout << "Error at opening file in Merge" << endl;
	    is_not_open = true;
	    break;
	}
	if (!f1.eof()) f1.read(reinterpret_cast<char*>(&left), sizeof(uint64_t)); else break;
	if (!f2.eof()) f2.read(reinterpret_cast<char*>(&right), sizeof(uint64_t)); else break;
	while (!f1.eof() && !f2.eof()) 
	{
	    uint64_t i = 0; // индексы сливаемых массивов
	    uint64_t j = 0;
	    while (i < count_sorted && j < count_sorted && !f1.eof() && !f2.eof())
            {
		if (left < right) 
                {
		    f.write(reinterpret_cast<char*>(&left), sizeof(uint64_t));
		    f1.read(reinterpret_cast<char*>(&left), sizeof(uint64_t));
		    i++;
		}
		else 
                {
		    f.write(reinterpret_cast<char*>(&right), sizeof(uint64_t));
		    f2.read(reinterpret_cast<char*>(&right), sizeof(uint64_t));
		    j++;
		}
	    }
	    while (i < count_sorted && !f1.eof()) 
            {
		f.write(reinterpret_cast<char*>(&left), sizeof(uint64_t));
		f1.read(reinterpret_cast<char*>(&left), sizeof(uint64_t));
		i++;
	    }
	    while (j < count_sorted && !f2.eof()) 
            {
		f.write(reinterpret_cast<char*>(&right), sizeof(uint64_t));
		f2.read(reinterpret_cast<char*>(&right), sizeof(uint64_t));
		j++;
	    }
	}
	while (!f1.eof()) 
        {
	    f.write(reinterpret_cast<char*>(&left), sizeof(uint64_t));
	    f1.read(reinterpret_cast<char*>(&right), sizeof(uint64_t));
	}
	while (!f2.eof()) 
        {
	    f.write(reinterpret_cast<char*>(&right), sizeof(uint64_t));
	    f2.read(reinterpret_cast<char*>(&right), sizeof(uint64_t));
	}
	f2.close();
	f1.close();
	f.close();
	count_sorted *= 2;
	dataReady = true;
	ready.notify_one();
    }
    if(!is_not_open)
    {
	f2.close();
	f1.close();
	f.close();
	remove("sort_left");
	remove("ssort_right");
    }
    complete = true;
    ready.notify_one();
}

int BatchSort(const char* name)
{
    uint64_t N = 100000;
    ifstream f1;
    ofstream f2;
    f1.open(name, ios::binary);
    f2.open("data_help.dat", ios::binary);
    if (!f1.is_open() || !f2.is_open())
    {
	cout << "Error at open file in BatchSort" << endl;
	return -1;
    }
    if (f1.eof())
	return -2;
    uint64_t  Sorted_Numbers = 0; // количество отсортированных чисел
    uint64_t* buf = new uint64_t[N];
    while (!f1.eof())
    {
	f1.read(reinterpret_cast<char*>(buf), sizeof(uint64_t)*N);
	Sorted_Numbers += f1.gcount()/ sizeof(uint64_t);
	sort(buf, buf + f1.gcount()/ sizeof(uint64_t));
	f2.write(reinterpret_cast<char*>(buf), f1.gcount());
    }
    delete[] buf;
    f1.close();
    f2.close();
    // dataReady - готовность данных для Separation
    // complete - готовность к завершению программы
    bool dataReady = true;
    bool complete = false;
    std::mutex mutex_;
    condition_variable ready;
    thread t1(Separation, "data_help.dat", Sorted_Numbers, N,  ref(dataReady), ref(complete), ref(mutex_), ref(ready));
    thread t2(Merge, "data_help.dat", Sorted_Numbers, N, ref(dataReady), ref(complete), ref(mutex_), ref(ready));
    t1.join();
    t2.join();
    remove("data_help.dat");
    return 0;
}

int main()
{
    cout << BatchSort("data.dat") << endl;
    return 0;
}
