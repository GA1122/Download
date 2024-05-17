bool ChildThread::ChildThreadMessageRouter::Send(IPC::Message* msg) {
  return sender_->Send(msg);
}
