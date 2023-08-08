#include <mutex>
#include <thread>
#include <iostream>
using namespace std;

#include "IncreaseValue.h"

int val = 0;
mutex mtx;

int main()
{
	thread t[10];
	for (thread& i : t)i = thread(IncreaseValue, &mtx, &val);
	for (thread& i : t)i.join();

	return 0;
}