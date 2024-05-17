void WebSocket::OnError(const SocketStream* socket_stream, int error) {
  origin_loop_->PostTask(FROM_HERE,
                         NewRunnableMethod(this, &WebSocket::DoError, error));
}
