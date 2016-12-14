// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�



#include "UIlib.h"

using namespace DuiLib;


#define		SUCCESS		0
#define		FAIL		-1


///////////
#define WM_PROGRESS_CLOSE		WM_APP+100

#define WM_ORDER_QUERY			WM_APP+101
#define WM_TICKET_QUERY			WM_APP+102


typedef enum 
{
	_GC,			///����
	_D,				///����
	_Z,				///ֱ��
	_T,				///�ؿ�
	_K,				///�տ�
	_O,				///����
} _TRAIN_TYPE;


typedef enum
{
	_ADULT,
	_STUDENT,
} _TICKET_TYPE;