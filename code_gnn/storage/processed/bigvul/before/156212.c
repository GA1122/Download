scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::InputTaskQueue() {
  helper_.CheckOnValidThread();
  return input_task_queue_;
}
