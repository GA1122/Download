void WebSocketJob::OnError(const SocketStream* socket, int error) {
  if (delegate_ && error != ERR_PROTOCOL_SWITCHED)
    delegate_->OnError(socket, error);
}
