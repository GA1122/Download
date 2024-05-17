scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::DefaultTaskQueue() {
  return helper_.DefaultMainThreadTaskQueue();
}
