RenderThreadImpl::GetFileThreadMessageLoopProxy() {
  DCHECK(message_loop() == MessageLoop::current());
  if (!file_thread_.get()) {
    file_thread_.reset(new base::Thread("Renderer::FILE"));
    file_thread_->Start();
  }
  return file_thread_->message_loop_proxy();
}
