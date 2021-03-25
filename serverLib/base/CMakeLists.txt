
set(HEADERS
  buffer.h
  circleBuffer.h
  circleReadBuffer.h
  circleWriteBuffer.h
  threadpool.h
  threadpoolSingleton.h
  timeStamp.h
)
install(FILES ${HEADERS} DESTINATION include/serverLib/base)