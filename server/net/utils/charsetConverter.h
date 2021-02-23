#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
namespace net{
    struct CharsetConverter{
    static std::string Encode(const std::string&);
    static std::string urlToUtf8(const std::string&);
    static std::string urlToUtf8(std::vector<char>::const_iterator, std::vector<char>::const_iterator);
    private:
        static const std::string & HEX_2_NUM_MAP();
        static const std::string & ASCII_EXCEPTION();
        static unsigned char NUM_2_HEX(const char h, const char l);
    };
}

