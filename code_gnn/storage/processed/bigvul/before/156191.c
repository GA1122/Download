base::TimeTicks RendererSchedulerImpl::EnableVirtualTime() {
  if (main_thread_only().use_virtual_time)
    return main_thread_only().initial_virtual_time;
  main_thread_only().use_virtual_time = true;
  DCHECK(!virtual_time_domain_);
  main_thread_only().initial_virtual_time = tick_clock()->NowTicks();
  virtual_time_domain_.reset(new AutoAdvancingVirtualTimeDomain(
      main_thread_only().initial_virtual_time, &helper_));
  RegisterTimeDomain(virtual_time_domain_.get());
  virtual_time_domain_->SetObserver(this);

  DCHECK(!virtual_time_control_task_queue_);
  virtual_time_control_task_queue_ =
      helper_.NewTaskQueue(MainThreadTaskQueue::QueueCreationParams(
          MainThreadTaskQueue::QueueType::kControl));
  virtual_time_control_task_queue_->SetQueuePriority(
      TaskQueue::kControlPriority);
  virtual_time_control_task_queue_->SetTimeDomain(virtual_time_domain_.get());

  main_thread_only().use_virtual_time = true;
  ForceUpdatePolicy();

  virtual_time_domain_->SetCanAdvanceVirtualTime(
      !main_thread_only().virtual_time_stopped);

  if (main_thread_only().virtual_time_stopped)
    VirtualTimePaused();
  return main_thread_only().initial_virtual_time;
}
