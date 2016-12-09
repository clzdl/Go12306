#pragma once

#include <string>

/*@action: utf8����Ķ��ֽڴ���ת����unicode��
 *
*/
std::wstring Utf8ToUnicode(const std::string &str);

/*@action: unicode����Ĵ���ת����utf8����Ķ��ֽڴ�
*
*/
std::string UnicodeToUtf8(const std::wstring &wstr);

/*@action:��Դ��ʼ��
*/
void InitResource();