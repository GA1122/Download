bool RenderThreadImpl::IsIOThread() {
  return MessageLoop::current() == ChildProcess::current()->io_message_loop();
}
