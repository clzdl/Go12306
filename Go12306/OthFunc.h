#pragma once

#include <string>
#include "zlib.h"

/*@action: utf8����Ķ��ֽڴ���ת����unicode��
 *
*/
std::wstring Utf8ToUnicode(const std::string &str);

/*@action: unicode����Ĵ���ת����utf8����Ķ��ֽڴ�
*
*/
std::string UnicodeToUtf8(const std::wstring &wstr);


/*@action: unicode����Ĵ���ת����gbk����Ķ��ֽڴ�
*
*/
std::string UnicodeToGbk(const std::wstring &wstr);


/*@action: ���ر���Ĵ���ת����utf8����Ķ��ֽڴ�
*
*/
std::wstring GbkToUnicode(const std::string &str);

/*@action: Gbk to utf8
*/

std::string Gbk2Utf8(const std::string str);

/*@action: Gbk to utf8
*/

std::string Utf8ToGbk(const std::string str);

/*@action:��Դ��ʼ��
*/
void InitResource();

/*@action: ѹ�������gzip��ʽ��
*/
int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);

/*@action: ��ѹ gzip��ѹ����  
*/
int Gunzip(Byte *zdata, uLong nzdata , std::string &gunZipString);



/*@action:
*/
long String2Number(std::string str);
