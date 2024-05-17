void WebSocket::OnReceivedData(SocketStream* socket_stream,
                               const char* data, int len) {
  DCHECK(socket_stream == socket_stream_);
  AddToReadBuffer(data, len);
  origin_loop_->PostTask(FROM_HERE,
                         NewRunnableMethod(this, &WebSocket::DoReceivedData));
}
