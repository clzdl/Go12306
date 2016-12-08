#pragma once

#include <string>


std::wstring Utf8ToUnicode(const std::string &str);

std::string UnicodeToUtf8(const std::wstring &wstr);
