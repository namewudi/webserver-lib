#include "httpUtils.h"

namespace net{
    const std::string HttpUtils::CRLFPattern = "\r\n";
    const std::string HttpUtils::spacePattern = " ";
    const std::string HttpUtils::parameterPattern = "?";
    const std::string HttpUtils::equalPattern = "=";
    const std::string HttpUtils::andPattern = "&";
    const std::string HttpUtils::colonPattern = ":";
    const std::string HttpUtils::http1_0Pattern = "HTTP/1.0";
    const std::string HttpUtils::http1_1Pattern = "HTTP/1.1";
    const std::string HttpUtils::getPattern = "GET";
    const std::string HttpUtils::postPattern = "POST";

    Status HttpUtils::parseRequest(std::shared_ptr<base::CircleReadBuffer>& data, std::shared_ptr<HttpRequest> req){
        
        int start = data->getIndex();
        int finish = data->size();
        //std::cerr<<"start parse request!"<<"from "<<start<<" to "<<finish<<std::endl;
        //std::cerr<<"row quest: "<<data->getString(start, finish)<<std::endl;
        int targetIndex = 0;
        int bodyLength = 0;
        int receivedBody = 0;
        int bodyCanRead = 0;
        Status curStatus = Status::init;
        do{
            switch (curStatus){
                case Status::init:
                    //std::cerr<<"start parse request line!"<<std::endl;
                    targetIndex  = data->find(start, finish, CRLFPattern);
                    if(targetIndex != -1){ 
                        req->setStatus(parseRequestLine(data, start, targetIndex, req->getRequestLine()));
                        data->setIndex(targetIndex); 
                        curStatus = req->getStatus();
                    }
                    else {
                        //std::cerr<<"data incomplete!"<<std::endl;
                        curStatus = Status::init;
                        break;
                    }
                    if(curStatus == Status::lineComplete) {
                        start = data->modifyIndex(targetIndex + 2);
                        data->setIndex(start);
                    }
                case Status::lineComplete:
                    //std::cerr<<"start parse header!"<<std::endl;
                    while(true){
                        targetIndex = data->find(start, finish, CRLFPattern);
                        if(targetIndex != -1){
                            req->setStatus(parseRequestHeader(data, start, targetIndex, req));
                            curStatus = req->getStatus();
                            if(curStatus == Status::headerComplete) {
                                //std::cerr<<"header parse finish!"<<std::endl;
                                start = data->modifyIndex(targetIndex + 2);
                                data->setIndex(start);
                                break;
                            }
                            else if(curStatus == Status::parseingHeader){
                                start = data->modifyIndex(targetIndex + 2);
                                data->setIndex(start);
                            }
                        }
                        else {
                            //std::cerr<<"header incomplete!"<<std::endl;
                            curStatus = Status::lineComplete;
                            break;
                        }
                    }
                case Status::headerComplete:
                    //std::cerr<<"start parse body!"<<std::endl;
                    switch (req->getMethod())
                    {
                    case HttpMethod::GET:
                        //std::cerr<<"get no body!"<<std::endl;
                        curStatus = Status::complete;
                        break;
                    case HttpMethod::POST:
                        if(bodyLength == 0){
                            if(req->hasHeader("Content-Length")){
                                bodyLength = std::stoi(req->getHeader("Content-Length"));
                                //std::cerr<<"body length: "<<bodyLength<<std::endl;
                                if(bodyLength == 0){
                                    curStatus = Status::complete;
                                    break;
                                }
                            }
                            else{
                                //bodyLength = data->byteToRead();
                                throw exception::HttpException("不包含Content-Length");
                            }
                            if(req->hasHeader("Content-Type")){
                                req->getRequestBody()->setContentType(req->getHeader("Content-Type"));
                                //std::cerr<<"Content-Type: "<<req->getHeader("Content-Type")<<std::endl;
                            }
                            else{
                                //req->getRequestBody()->setContentType("application/x-www-form-urlencoded");
                                throw exception::HttpException("不包含Content-Type");
                            }
                        }
                        
                        bodyCanRead = std::min(data->byteToRead(), bodyLength - receivedBody);
                        //std::cerr<<"bodyCanRead: "<<bodyCanRead<<std::endl;
                        if(bodyCanRead == 0){
                            curStatus == Status::headerComplete;
                            break;
                        }
                        //std::cerr<<"新加入body的数据为"<<data->getString(data->modifyIndex(start + receivedBody), data->modifyIndex(start + receivedBody + bodyCanRead))<<std::endl;
                        req->getRequestBody()->append(data->getString(data->modifyIndex(start + receivedBody), data->modifyIndex(start + receivedBody + bodyCanRead)));
                        receivedBody += bodyCanRead;
                        data->setIndex(data->modifyIndex(start + receivedBody));
                        if(bodyLength == receivedBody) {
                            req->setStatus(parseRequestBody(req->getRequestBody()));                           
                            curStatus = req->getStatus();
                            break;
                        } 
                    }
                default:
                    finish = data->size();
            }
        }
        while(curStatus != Status::complete && curStatus != Status::error);
        
        //std::cerr<<"parse request finished!"<<std::endl;
        return curStatus;
    }
    Status HttpUtils::parseRequestLine(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end, std::shared_ptr<HttpRequestLine> reqLine){
        
        int start = begin;
        int targetIndex = data->find(begin, end, spacePattern);
        if(targetIndex != -1) {
            reqLine->setMethod(parseMethod(data, begin, targetIndex));
            start = data->modifyIndex(targetIndex + 1);
        }
        else{
            return Status::error;
        }
        //std::cerr<<"start parse url!"<<std::endl;
        targetIndex = data->find(start, end, parameterPattern);
        if(targetIndex != -1) {
            reqLine->setUrl(data->getString(start, targetIndex));
            //std::cerr<<"parse url result: "<<reqLine->getUrl()<<std::endl;
            start = data->modifyIndex(targetIndex + 1);
            targetIndex = data->find(start, end, spacePattern);
            //std::cerr<<"url contains parameter!"<<std::endl;
            if(targetIndex != -1) {
                parseParameter(data, start, targetIndex, [&reqLine](const std::string& k, const std::string& v){reqLine->setParameter(k, v);});
                start = data->modifyIndex(targetIndex + 1);
            }
            else{
                return Status::error;
            }
        }
        else {
            //std::cerr<<"url do not contains parameter!"<<std::endl;
            targetIndex = data->find(start, end, spacePattern);
            if(targetIndex != -1) {
                reqLine->setUrl(data->getString(start, targetIndex));
                //std::cerr<<"parse url result: "<<reqLine->getUrl()<<std::endl;
                start = data->modifyIndex(targetIndex + 1);
            }
            else{
                return Status::error;
            }
        }
        reqLine->setHttpVersion(parseVersion(data->getString(start, end)));
        //std::cerr<<"parse request line finished!"<<std::endl;
        return Status::lineComplete;
    }
            
    HttpVersion HttpUtils::parseVersion(std::string version){
        //std::cerr<<"start parse version!"<<std::endl;
        HttpVersion result;
        if(version == http1_0Pattern) {
            //std::cerr<<"version :"<<"http/1.0"<<std::endl;
            result = HttpVersion::HTTP1_0;
        }
        else if(version == http1_1Pattern){
            //std::cerr<<"version :"<<"http/1.1"<<std::endl;
            result = HttpVersion::HTTP1_1;
        }
        else{
            throw exception::HttpVersionException();
        }
        return result;
    }
    void HttpUtils::parseParameter(std::shared_ptr<base::CircleReadBuffer>& data, int first, int last, std::function<void(const std::string&, const std::string&)> setParam){
        std::string utf8Parameter = CharsetConverter::urlToUtf8(data->getString(first, last));
        parseParameter(utf8Parameter, std::move(setParam));
    }
    void HttpUtils::parseParameter(const std::string& data, std::function<void(const std::string&, const std::string&)> setParam){
        //std::cerr<<"start parse parameter!"<<std::endl;
        //std::cerr<<"row parameter is: "<<data<<std::endl;
        size_t start = 0;
        while(true){
            size_t index = data.find(equalPattern, start);
            if(index == std::string::npos){
                break;
            }
            std::string key(data.substr(start, index - start));
            start = index + 1;
            index = data.find(andPattern, start);
            if(index == std::string::npos){
                setParam(CharsetConverter::urlToUtf8(key), CharsetConverter::urlToUtf8(data.substr(start, data.length() - start)));
                break;
            }
            else{
                setParam(CharsetConverter::urlToUtf8(key), CharsetConverter::urlToUtf8(data.substr(start, index - start)));
            }
            start = index + 1;
        }
        //std::cerr<<"parse parameter finished!"<<std::endl;
    }
    HttpMethod HttpUtils::parseMethod(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end){
        //std::cerr<<"start parse method!"<<std::endl;
        HttpMethod result;
        if(data->find(begin, end, getPattern) != -1){
            //std::cerr<<"method :"<<"GET!"<<std::endl;
            result = HttpMethod::GET;
        }
        else if(data->find(begin, end, postPattern) != -1){
            //std::cerr<<"method :"<<"POST!"<<std::endl;
            result = HttpMethod::POST;
        }
        else {
            throw exception::HttpMethodException(data->getString(begin, end).c_str());
        }
        //std::cerr<<"parse method finished"<<std::endl;
        return result;
    }
    Status HttpUtils::parseRequestHeader(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end, std::shared_ptr<HttpRequest> req){
        if(begin == end) return Status::headerComplete;
        int targetIndex = data->find(begin, end, colonPattern);
        if(targetIndex == -1) return Status::error;
        req->setHeader(data->getString(begin, targetIndex), data->getString(targetIndex + 2, end));
        //std::cerr<<"parse header: "<<data->getString(begin, targetIndex)<<" : "<<data->getString(targetIndex + 2, end)<<std::endl;
        return Status::parseingHeader;
    }
    Status HttpUtils::parseRequestBody(std::shared_ptr<HttpRequestBody> reqBody){
        std::string data = reqBody->getAsString();
        //std::cerr<<"row body is :"<<data<<std::endl;
        if(reqBody->getContentType() == ContentType::application_x_www_form_urlencoded){
            parseParameter(data, [&reqBody](const std::string& k, const std::string& v){reqBody->setParameter(k, v);});
        }
        else if(reqBody->getContentType() == ContentType::multipart_form_data){
            //std::cerr<<"parse multipart_form_data"<<std::endl;
            parseMultiParameter(data, reqBody);
        }
        //std::cerr<<"parse request body finished!"<<std::endl;
        return Status::complete;
    }
    void HttpUtils::parseMultiParameter(const std::string& data, std::shared_ptr<HttpRequestBody> reqBody){
        int start = data.find(reqBody->getBoundary());
        if(start == data.length()) return;
        start += (reqBody->getBoundary().size() + 2);
        int target = 0;
        int end;
        while(true){
            end = data.find(reqBody->getBoundary(), start);
            if(end == std::string::npos) break;
            MultiPart multiPart;
            while(true){
                target = data.find(CRLFPattern, start);
                if(target == start) {
                    multiPart.setData(data.substr(start + 2, end - 4 - start)); //fix me
                    start = end + reqBody->getBoundary().size() + 2;
                    break;
                }
                else{
                    int colon = data.find(colonPattern, start);
                    multiPart.setHeader(data.substr(start, colon - start), data.substr(colon + 2, target - 2 - colon));
                    start = target + 2;
                }
            }
            reqBody->addMultiPart(std::move(multiPart));
        }
    }
    const std::string HttpUtils::parseToString(const HttpMethod& method){
        switch(method){
            case HttpMethod::GET:
            return getPattern;
            case HttpMethod::POST:
            return postPattern;
            default:
            throw exception::HttpMethodException();
        }
        return "";
    }
    const std::string HttpUtils::parseToString(const HttpVersion& version){
        switch(version){
            case HttpVersion::HTTP1_0:
            return http1_0Pattern;
            case HttpVersion::HTTP1_1:
            return http1_1Pattern;
            default:
            throw exception::HttpVersionException();
        }
        return "";
    }
}