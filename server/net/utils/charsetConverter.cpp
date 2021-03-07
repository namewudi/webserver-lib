#include "charsetConverter.h"
namespace net{
    const std::string & CharsetConverter::HEX_2_NUM_MAP()
    {
        static const std::string str("0123456789ABCDEF");
        return str;
    }

    const std::string & CharsetConverter::ASCII_EXCEPTION()
    {
        static const std::string str(R"("%<>[\]^_`{|})");
        return str;
    }

    unsigned char CharsetConverter::NUM_2_HEX(const char h, const char l)
    {
        unsigned char hh = std::find(std::begin(HEX_2_NUM_MAP()), std::end(HEX_2_NUM_MAP()), h) - std::begin(HEX_2_NUM_MAP());
        unsigned char ll = std::find(std::begin(HEX_2_NUM_MAP()), std::end(HEX_2_NUM_MAP()), l) - std::begin(HEX_2_NUM_MAP());
        return (hh << 4) + ll;
    }

    std::string CharsetConverter::Encode(const std::string & url)
    {
        std::string ret;
        for (auto it = url.begin(); it != url.end(); ++it)
        {
            if (((*it >> 7) & 1) || (std::count(std::begin(ASCII_EXCEPTION()), std::end(ASCII_EXCEPTION()), *it)))
            {
                ret.push_back('%');
                ret.push_back(HEX_2_NUM_MAP()[(*it >> 4) & 0x0F]);
                ret.push_back(HEX_2_NUM_MAP()[*it & 0x0F]);
            }
            else
            {
                ret.push_back(*it);
            }
        }
        return ret;
    }

    std::string CharsetConverter::urlToUtf8(const std::string & url)
    {
        std::string ret;
        
        for (auto iter = url.begin(); iter != url.end(); ++iter){
            if (*iter == '%'){
                if((iter + 1) == url.end() || (iter + 2) == url.end()){
                    throw std::invalid_argument("url is invalid");
                }
                
                ret.push_back(NUM_2_HEX(*(iter + 1), *(iter + 2)));
                iter += 2;
            }
            else{
                ret.push_back(costumConverter(*iter));
            }
        }
        return ret;
    }

    std::string CharsetConverter::urlToUtf8(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end)
    {
        std::string ret;
        for (auto iter = begin; iter != end; ++iter){
            if (*iter == '%'){
                if((iter + 1) == end || (iter + 2) == end){
                    throw std::invalid_argument("url is invalid");
                }
                ret.push_back(NUM_2_HEX(*(iter + 1), *(iter + 2)));
                iter += 2;
            }
            else{
                ret.push_back(costumConverter(*iter));
            }
        }
        return ret;
    }
    char CharsetConverter::costumConverter(char c){
        if(c == '+') {
            return ' ';
        }
        return c;
    }
}
