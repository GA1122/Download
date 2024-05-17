void SocketStreamDispatcherHost::OnClose(net::SocketStream* socket) {
  int socket_id = SocketStreamHost::SocketIdFromSocketStream(socket);
  DVLOG(1) << "SocketStreamDispatcherHost::OnClosed socket_id=" << socket_id;
  if (socket_id == content::kNoSocketId) {
    LOG(ERROR) << "NoSocketId in OnClose";
    return;
  }
  DeleteSocketStreamHost(socket_id);
}
