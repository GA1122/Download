void WebSocketJob::OnClose(SocketStream* socket) {
  state_ = CLOSED;
  WebSocketThrottle::GetInstance()->RemoveFromQueue(this);
  WebSocketThrottle::GetInstance()->WakeupSocketIfNecessary();

  scoped_refptr<WebSocketJob> protect(this);

  SocketStream::Delegate* delegate = delegate_;
  delegate_ = NULL;
  socket_ = NULL;
  if (callback_) {
    waiting_ = false;
    callback_ = NULL;
    Release();   
  }
  if (delegate)
    delegate->OnClose(socket);
}
