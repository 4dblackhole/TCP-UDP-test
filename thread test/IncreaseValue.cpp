#include <mutex>
#include <thread>

#include <iostream>
using namespace std;

#include "IncreaseValue.h"

// can't use reference(&) as a parameter
/******************************************************************************
*	int main()
*	{
*		{
*			int a;
*			thread t = thread(func, a);
*			th.detach();
*		}
*		// "a" is out of scope
*		// at this point the thread may be still running
*	}
* 
******************************************************************************/

void IncreaseValue(mutex* mtx, int* i)
{
	mutex& m = *mtx;
	int& idx = *i;
	m.lock();
	idx++;
	cout << idx << endl;
	m.unlock();
}
