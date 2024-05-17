RendererSchedulerImpl::CompositorTaskQueue() {
  helper_.CheckOnValidThread();
  return compositor_task_queue_;
}
