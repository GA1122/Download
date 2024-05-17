void WebSocket::OnClose(SocketStream* socket_stream) {
  origin_loop_->PostTask(FROM_HERE,
                         NewRunnableMethod(this, &WebSocket::DoClose));
}
