MessageRouter* ChildThread::GetRouter() {
  DCHECK(base::MessageLoop::current() == message_loop());
  return &router_;
}
