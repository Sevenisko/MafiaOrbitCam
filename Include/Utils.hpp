#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

namespace Utils {
    static std::string LowerString(const std::string& src) {
        std::string str(src);

        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return src;
    }

    static std::string UpperString(const std::string& src) {
        std::string str(src);

        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return std::toupper(c);
        });

        return str;
    }

    static std::string ReplaceString(const std::string& src, const std::string& target, const std::string& newStr) {
        std::string str(src);

        size_t start_pos = 0;
        while ((start_pos = str.find(target, start_pos)) != std::string::npos) {
            str.replace(start_pos, str.size(), newStr);
            start_pos += newStr.size();
        }

        return str;
    }

    static std::string GetModPath() {
        HMODULE mod = GetModuleHandleA("MafiaOrbitCam.asi");

        if (mod) {
            std::string path;
            path.resize(MAX_PATH);

            char* pPath = (char*)path.data();

            int len = GetModuleFileNameA(mod, pPath, MAX_PATH);

            if (len != 0) {
                size_t pos = path.find_last_of("\\");

                if (pos != std::string::npos) {
                    return path.substr(0, pos);
                }
            }
        }

        return "";
    }

    /*static std::vector<std::string> SplitString(std::string s, std::string delimiter = " ") {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token     = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }*/

    static std::string TrimString(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) {
            return "";
        }
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    }

    /*std::string JoinString(const std::vector<std::string>& strings, const std::string& delimiter = " ") {
        std::ostringstream oss;
        for (size_t i = 0; i < strings.size(); ++i) {
            if (i != 0) {
                oss << delimiter;
            }
            oss << strings[i];
        }
        return oss.str();
    }*/
} // namespace Utils