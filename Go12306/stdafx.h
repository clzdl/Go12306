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



#include "UIlib.h"

using namespace DuiLib;


#define		SUCCESS		0
#define		FAIL		-1


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