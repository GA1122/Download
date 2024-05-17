int WebSocketJob::OnStartOpenConnection(
    SocketStream* socket, CompletionCallback* callback) {
  DCHECK(!callback_);
  state_ = CONNECTING;
  addresses_ = socket->address_list();
  WebSocketThrottle::GetInstance()->PutInQueue(this);
  if (delegate_) {
    int result = delegate_->OnStartOpenConnection(socket, callback);
    DCHECK_EQ(OK, result);
  }
  if (waiting_) {
    callback_ = callback;
    AddRef();   
    return ERR_IO_PENDING;
  }
  return TrySpdyStream();
}
