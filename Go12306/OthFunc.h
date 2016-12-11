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


/*@action: ���ر���Ĵ���ת����utf8����Ķ��ֽڴ�
*
*/
std::wstring GbkToUnicode(const std::string &str);

/*@action: Gbk to utf8
*/

std::string Gbk2Utf8(const std::string str);

/*@action:��Դ��ʼ��
*/
void InitResource();