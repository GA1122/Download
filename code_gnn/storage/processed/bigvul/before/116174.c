void SocketStreamDispatcherHost::CancelSSLRequest(
    const content::GlobalRequestID& id,
    int error,
    const net::SSLInfo* ssl_info) {
  int socket_id = id.request_id;
  DVLOG(1) << "SocketStreamDispatcherHost::CancelSSLRequest socket_id="
           << socket_id;
  DCHECK_NE(content::kNoSocketId, socket_id);
  SocketStreamHost* socket_stream_host = hosts_.Lookup(socket_id);
  DCHECK(socket_stream_host);
  if (ssl_info)
    socket_stream_host->CancelWithSSLError(*ssl_info);
  else
    socket_stream_host->CancelWithError(error);
}
