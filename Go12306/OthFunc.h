#pragma once

#include <string>
#include "zlib.h"

/*@action: utf8编码的多字节串，转换成unicode串
 *
*/
std::wstring Utf8ToUnicode(const std::string &str);

/*@action: unicode编码的串，转换成utf8编码的多字节串
*
*/
std::string UnicodeToUtf8(const std::wstring &wstr);


/*@action: unicode编码的串，转换成gbk编码的多字节串
*
*/
std::string UnicodeToGbk(const std::wstring &wstr);


/*@action: 本地编码的串，转换成utf8编码的多字节串
*
*/
std::wstring GbkToUnicode(const std::string &str);

/*@action: Gbk to utf8
*/

std::string Gbk2Utf8(const std::string str);

/*@action: Gbk to utf8
*/

std::string Utf8ToGbk(const std::string str);

/*@action:资源初始化
*/
void InitResource();

/*@action: 压缩输出成gzip格式流
*/
int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);

/*@action: 解压 gzip的压缩流  
*/
int Gunzip(Byte *zdata, uLong nzdata , std::string &gunZipString);



/*@action:
*/
long String2Number(std::string str);
