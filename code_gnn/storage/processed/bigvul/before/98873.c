void WebSocket::DoClose() {
  DCHECK(MessageLoop::current() == origin_loop_);
  WebSocketDelegate* delegate = delegate_;
  delegate_ = NULL;
  ready_state_ = CLOSED;
  if (!socket_stream_)
    return;
  socket_stream_ = NULL;
  if (delegate)
    delegate->OnClose(this);
  Release();
}
