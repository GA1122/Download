bool NaClListener::Send(IPC::Message* msg) {
  DCHECK(main_loop_ != NULL);
  if (MessageLoop::current() == main_loop_) {
    return channel_->Send(msg);
  } else {
    return filter_->Send(msg);
  }
}
