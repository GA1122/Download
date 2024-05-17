void WebSocket::Send(const std::string& msg) {
  DCHECK(ready_state_ == OPEN);
  DCHECK(MessageLoop::current() == origin_loop_);

  IOBufferWithSize* buf = new IOBufferWithSize(msg.size() + 2);
  char* p = buf->data();
  *p = '\0';
  memcpy(p + 1, msg.data(), msg.size());
  *(p + 1 + msg.size()) = '\xff';
  pending_write_bufs_.push_back(buf);
  SendPending();
}
