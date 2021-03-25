
set(HEADERS
cookie.h
filter.h
filterChain.h
filterManager.h
httpException.h
httprequest.h
httpRequestBody.h
httpRequestHeader.h
httpRequestLine.h
httpresponse.h
httpserver.h
httpSession.h
httpSessionFactory.h
httpSessionLocal.h
httpSessionRedis.h
multiPart.h
servlet.h
servletManager.h
)
install(FILES ${HEADERS} DESTINATION include/serverLib/net/http)