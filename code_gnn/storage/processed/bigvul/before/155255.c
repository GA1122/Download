bool ConnectToHandler(CrashReporterClient* client, base::ScopedFD* connection) {
  int fds[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != 0) {
    PLOG(ERROR) << "socketpair";
    return false;
  }
  base::ScopedFD local_connection(fds[0]);
  base::ScopedFD handlers_socket(fds[1]);

  if (!HandlerStarter::Get()->StartHandlerForClient(client,
                                                    handlers_socket.get())) {
    return false;
  }

  *connection = std::move(local_connection);
  return true;
}
