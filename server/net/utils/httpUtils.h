#pragma once
#include <iostream>
#include <string>
#include "charsetConverter.h"
#include <algorithm>
#include <unordered_map>
#include "../../exception/httpException.h"
#include "../httpRequest/httprequest.h"
#include "processStatus.h"
#include "../../base/circleReadBuffer.h"
#include "assert.h"

namespace net{
    class HttpUtils{
        HttpUtils() = delete;
        using dataIterator = std::vector<char>::const_iterator;
    public:
        static Status parseRequest(std::shared_ptr<base::CircleReadBuffer>& data, std::shared_ptr<HttpRequest> req);
        static Status parseRequestLine(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end, std::shared_ptr<HttpRequestLine> reqLine);
        static HttpVersion parseVersion(std::string);
        static void parseParameter(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end, std::function<void(const std::string&, const std::string&)>);
        static void parseParameter(const std::string& data, std::function<void(const std::string&, const std::string&)>);
        static HttpMethod parseMethod(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end);
        static Status parseRequestHeader(std::shared_ptr<base::CircleReadBuffer>& data, int begin, int end, std::shared_ptr<HttpRequest> req);
        static Status parseRequestBody(std::shared_ptr<HttpRequestBody> reqBody);
        static const std::string parseToString(const HttpMethod&);
        static const std::string parseToString(const HttpVersion&);
    private:
        static const std::string CRLFPattern;
        static const std::string spacePattern;
        static const std::string parameterPattern;
        static const std::string equalPattern;
        static const std::string andPattern;
        static const std::string colonPattern;
        static const std::string http1_0Pattern;
        static const std::string http1_1Pattern;
        static const std::string getPattern;
        static const std::string postPattern;
    };
}