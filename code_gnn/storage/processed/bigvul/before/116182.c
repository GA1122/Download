void SocketStreamDispatcherHost::OnReceivedData(
    net::SocketStream* socket, const char* data, int len) {
  int socket_id = SocketStreamHost::SocketIdFromSocketStream(socket);
  DVLOG(1) << "SocketStreamDispatcherHost::OnReceiveData socket_id="
           << socket_id;
  if (socket_id == content::kNoSocketId) {
    LOG(ERROR) << "NoSocketId in OnReceivedData";
    return;
  }
  if (!Send(new SocketStreamMsg_ReceivedData(
          socket_id, std::vector<char>(data, data + len)))) {
    LOG(ERROR) << "SocketStreamMsg_ReceivedData failed.";
    DeleteSocketStreamHost(socket_id);
  }
}
