#include <iostream>
#include <unordered_map>
namespace net{
    class HttpRequestHeader{
    public:
        using HeaderMap = std::unordered_map<std::string, std::string>;
        void setHeader(std::string key, std::string value){
            this->headerMap[key] = value;
        }
        const std::string getHeader(const std::string& key)const{
            return headerMap.at(key);
        }
        bool hasHeader(const std::string& key)const{
            return headerMap.count(key) > 0;
        }
        const HeaderMap getHeaderMap()const{
            return this->headerMap;
        }
    private:
        HeaderMap headerMap;
    };
}