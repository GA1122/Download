void SocketStreamDispatcherHost::OnConnected(net::SocketStream* socket,
                                             int max_pending_send_allowed) {
  int socket_id = SocketStreamHost::SocketIdFromSocketStream(socket);
  DVLOG(1) << "SocketStreamDispatcherHost::OnConnected socket_id=" << socket_id
           << " max_pending_send_allowed=" << max_pending_send_allowed;
  if (socket_id == content::kNoSocketId) {
    LOG(ERROR) << "NoSocketId in OnConnected";
    return;
  }
  if (!Send(new SocketStreamMsg_Connected(
          socket_id, max_pending_send_allowed))) {
    LOG(ERROR) << "SocketStreamMsg_Connected failed.";
    DeleteSocketStreamHost(socket_id);
  }
}
