void WebSocket::DoError(int error) {
  DCHECK(MessageLoop::current() == origin_loop_);
  if (delegate_)
    delegate_->OnError(this, error);
}
