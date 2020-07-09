#pragma once

#include <Windows.h>

class MyTime
{
public:
	static int i() {
		return 0;
	}
};

long long Subtract(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl);