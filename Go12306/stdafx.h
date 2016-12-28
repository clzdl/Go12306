// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件

////转换错误码定义
#define _ERRO_MAP(X)                               \
    X(OK, _T("0000") , _T("成功"))                         \
    X(FAILURE,_T("0002") , _T("业务处理失败"))             \
    X(LOGIN_REDIRECT, _T("0003") , _T("重定向到登录页面"))                    \
	X(CEHCK_USER, _T("0004") , _T("用户校验失败"))    \
	X(ERR_REDIRECT, _T("0005") , _T("重定向到错误页面"))    \



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
	_GC,			///高铁
	_D,				///动车
	_Z,				///直达
	_T,				///特快
	_K,				///普快
	_O,				///其它
} _TRAIN_TYPE;


typedef enum
{
	_ADULT,
	_STUDENT,
} _TICKET_TYPE;


/////////12306 座位信息定义
#define  _SEAT_TYPE_YDZ		_T("M")					////一等座
#define  _SEAT_TYPE_EDZ		_T("O")					////二等座
#define  _SEAT_TYPE_TDZ		_T("P")					////特等座
#define  _SEAT_TYPE_SWZ		_T("9")					////商务座

#define  _SEAT_TYPE_YW		_T("3")					////硬卧
#define  _SEAT_TYPE_RW		_T("4")					////软卧
#define  _SEAT_TYPE_YZ		_T("1")					////硬座
#define	 _SEAT_TYPE_RZ		_T("2")					////软座
#define  _SEAT_TYPE_GJRW	_T("6")					////高级软卧



///////12306 证件信息定义
#define  _CARD_TYPE_SFZ		_T("1")					////省份证
#define  _CARD_TYPE_GAZ		_T("C")					////港澳通行证
#define  _CARD_TYPE_TWZ		_T("G")					////台湾通行证
#define  _CARD_TYPE_HZ		_T("B")					////护照


///////12306  票类型信息定义
#define	 _TICKET_TYPE_CRP	_T("1")					/////成人票
#define	 _TICKET_TYPE_ETP	_T("2")					/////儿童票
#define	 _TICKET_TYPE_XSP	_T("3")					/////学生票
#define	 _TICKET_TYPE_CJP	_T("4")					/////残军票


