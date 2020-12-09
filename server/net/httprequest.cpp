#include "httprequest.h"

namespace net{
    bool HttpRequest::phaseRequestLine(int begin, int end){
        std::string space(" ");
        int start = begin;
        auto iter = std::find_first_of(_data.begin() + start, _data.begin() + end, space.begin(), space.end());
        if(iter != _data.begin() + end) {
            _method.assign(_data.begin() + start, iter);
            start = iter - _data.begin() + 1;
        }
        else{
            return false;
        }
        iter = std::find_first_of(_data.begin() + start, _data.begin() + end, space.begin(), space.end());
        if(iter != _data.begin() + end) {
            _url.assign(_data.begin() + start, iter);
            start = iter - _data.begin() + 1;
        }
        else{
            return false;
        }
        if((end - start) != 8) return false;
        _version = "HTTP/1.0";
        if(_data[end - 1] == '1') _version.back() = '1';
        return true;
    }
    bool HttpRequest::phaseRequestHeader(int begin, int end){
        if(begin == end) return false;
        std::string target(":");
        int idx = std::find_first_of(_data.begin() + begin, _data.begin() + end, target.begin(), target.end()) - _data.begin();
        if(idx == end) return false;
        _headerMap[std::string(_data.begin() + begin, _data.begin() + idx)] = std::string(_data.begin() + idx + 1, _data.begin() + end);
        return true;
    }
    bool HttpRequest::phase(){
        std::string CRLF("\r\n");
        int start = 0;
        int finish = _data.find_first_of(CRLF, start);
        if(!phaseRequestLine(start, finish)) return false;
        start = finish + 2;
        while(true){
            finish = _data.find_first_of(CRLF, start);
            if(!phaseRequestHeader(start, finish)) break;
            start = finish + 2;
        }
        return true;
    }
    const std::string HttpRequest::getHeader(const std::string& s) const{
        return _headerMap.at(s);
    }
    const std::string HttpRequest::getMethod()const{
        return _method;
    }
    const std::string HttpRequest::getUrl()const{
        return _url;
    }
    const std::string HttpRequest::getVersion()const{
        return _version;
    }
}