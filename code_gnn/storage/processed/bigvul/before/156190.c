void RendererSchedulerImpl::DisableVirtualTimeForTesting() {
  if (!main_thread_only().use_virtual_time)
    return;
  main_thread_only().use_virtual_time = false;

  if (main_thread_only().virtual_time_stopped) {
    main_thread_only().virtual_time_stopped = false;
    VirtualTimeResumed();
  }

  ForceUpdatePolicy();

  virtual_time_control_task_queue_->ShutdownTaskQueue();
  virtual_time_control_task_queue_ = nullptr;
  UnregisterTimeDomain(virtual_time_domain_.get());
  virtual_time_domain_.reset();
  virtual_time_control_task_queue_ = nullptr;
  ApplyVirtualTimePolicy();
}
