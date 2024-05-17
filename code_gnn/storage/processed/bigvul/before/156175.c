scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::ControlTaskQueue() {
  helper_.CheckOnValidThread();
  return helper_.ControlMainThreadTaskQueue();
}
