  void set_wakeup_policy(TaskQueueManager::WakeupPolicy wakeup_policy) {
    DCHECK(main_thread_checker_.CalledOnValidThread());
    wakeup_policy_ = wakeup_policy;
  }
