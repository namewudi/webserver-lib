
set(HEADERS
  acceptor.h
  acceptorV4.h
  acceptorV6.h
  channel.h
  epollpoller.h
  event.h
  managerPool.h
  poller.h
  socketinfor.h
  tcpconnection.h
  tcpserver.h
)
install(FILES ${HEADERS} DESTINATION include/serverLib/net/tcp)