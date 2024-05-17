void SocketStreamDispatcherHost::OnSendData(
    int socket_id, const std::vector<char>& data) {
  DVLOG(1) << "SocketStreamDispatcherHost::OnSendData socket_id=" << socket_id;
  SocketStreamHost* socket_stream_host = hosts_.Lookup(socket_id);
  if (!socket_stream_host) {
    LOG(ERROR) << "socket_id=" << socket_id << " already closed.";
    return;
  }
  if (!socket_stream_host->SendData(data)) {
    socket_stream_host->Close();
  }
}
