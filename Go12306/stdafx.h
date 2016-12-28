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

////ת�������붨��
#define _ERRO_MAP(X)                               \
    X(OK, _T("0000") , _T("�ɹ�"))                         \
    X(FAILURE,_T("0002") , _T("ҵ����ʧ��"))             \
    X(LOGIN_REDIRECT, _T("0003") , _T("�ض��򵽵�¼ҳ��"))                    \
	X(CEHCK_USER, _T("0004") , _T("�û�У��ʧ��"))    \
	X(ERR_REDIRECT, _T("0005") , _T("�ض��򵽴���ҳ��"))    \



#define  _ERR_GEN(n, c , s)		E_##n,

typedef enum
{
	_ERRO_MAP(_ERR_GEN)
} _ERRNO;

#undef _ERRNO_GEN


#include "UIlib.h"

using namespace DuiLib;




///////////
#define WM_PROGRESS_CLOSE		WM_APP+100

#define WM_ORDER_QUERY					WM_APP+101
#define WM_TICKET_QUERY					WM_APP+102
#define WM_POLL_TICKET_PROCESS			WM_APP+103

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


/////////12306 ��λ��Ϣ����
#define  _SEAT_TYPE_YDZ		_T("M")					////һ����
#define  _SEAT_TYPE_EDZ		_T("O")					////������
#define  _SEAT_TYPE_TDZ		_T("P")					////�ص���
#define  _SEAT_TYPE_SWZ		_T("9")					////������

#define  _SEAT_TYPE_YW		_T("3")					////Ӳ��
#define  _SEAT_TYPE_RW		_T("4")					////����
#define  _SEAT_TYPE_YZ		_T("1")					////Ӳ��
#define	 _SEAT_TYPE_RZ		_T("2")					////����
#define  _SEAT_TYPE_GJRW	_T("6")					////�߼�����



///////12306 ֤����Ϣ����
#define  _CARD_TYPE_SFZ		_T("1")					////ʡ��֤
#define  _CARD_TYPE_GAZ		_T("C")					////�۰�ͨ��֤
#define  _CARD_TYPE_TWZ		_T("G")					////̨��ͨ��֤
#define  _CARD_TYPE_HZ		_T("B")					////����


///////12306  Ʊ������Ϣ����
#define	 _TICKET_TYPE_CRP	_T("1")					/////����Ʊ
#define	 _TICKET_TYPE_ETP	_T("2")					/////��ͯƱ
#define	 _TICKET_TYPE_XSP	_T("3")					/////ѧ��Ʊ
#define	 _TICKET_TYPE_CJP	_T("4")					/////�о�Ʊ


