void SocketStreamDispatcherHost::DeleteSocketStreamHost(int socket_id) {
  SocketStreamHost* socket_stream_host = hosts_.Lookup(socket_id);
  DCHECK(socket_stream_host);
  delete socket_stream_host;
  hosts_.Remove(socket_id);
  if (!Send(new SocketStreamMsg_Closed(socket_id))) {
    LOG(ERROR) << "SocketStreamMsg_Closed failed.";
  }
}
