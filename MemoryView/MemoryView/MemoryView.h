
// MemoryView.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMemoryViewApp:
// �йش����ʵ�֣������ MemoryView.cpp
//

class CMemoryViewApp : public CWinApp
{
public:
	CMemoryViewApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMemoryViewApp theApp;