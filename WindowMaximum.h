
// WindowMaximum.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWindowMaximumApp: 
// �йش����ʵ�֣������ WindowMaximum.cpp
//

class CWindowMaximumApp : public CWinApp
{
public:
	CWindowMaximumApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWindowMaximumApp theApp;