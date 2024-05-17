RendererSchedulerImpl::VirtualTimeControlTaskQueue() {
  helper_.CheckOnValidThread();
  return virtual_time_control_task_queue_;
}
