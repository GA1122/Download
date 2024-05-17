void SocketStreamDispatcherHost::OnConnect(int render_view_id,
                                           const GURL& url,
                                           int socket_id) {
  DVLOG(1) << "SocketStreamDispatcherHost::OnConnect"
           << " render_view_id=" << render_view_id
           << " url=" << url
           << " socket_id=" << socket_id;
  DCHECK_NE(content::kNoSocketId, socket_id);
  if (hosts_.Lookup(socket_id)) {
    LOG(ERROR) << "socket_id=" << socket_id << " already registered.";
    return;
  }
  SocketStreamHost* socket_stream_host =
      new SocketStreamHost(this, render_view_id, socket_id);
  hosts_.AddWithID(socket_stream_host, socket_id);
  socket_stream_host->Connect(url, GetURLRequestContext());
  DVLOG(1) << "SocketStreamDispatcherHost::OnConnect -> " << socket_id;
}
