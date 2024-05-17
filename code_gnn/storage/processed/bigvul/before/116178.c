void SocketStreamDispatcherHost::OnCloseReq(int socket_id) {
  DVLOG(1) << "SocketStreamDispatcherHost::OnCloseReq socket_id=" << socket_id;
  SocketStreamHost* socket_stream_host = hosts_.Lookup(socket_id);
  if (!socket_stream_host)
    return;
  socket_stream_host->Close();
}
