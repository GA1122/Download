scoped_refptr<MainThreadTaskQueue> RendererSchedulerImpl::V8TaskQueue() {
  helper_.CheckOnValidThread();
  return v8_task_queue_;
}
