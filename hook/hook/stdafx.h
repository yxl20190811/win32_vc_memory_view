// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

const char *const CrtDllName = "msvcr90.dll";

const long long CallStackFlag = 0x1234567890abcdef;
const int kMaxCallers = 62; 

class TCallStack
{
public:
	long long m_time;
	long long m_deep;
	long long m_flag;
};

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
