  TaskQueueManager::WakeupPolicy wakeup_policy() const {
    DCHECK(main_thread_checker_.CalledOnValidThread());
    return wakeup_policy_;
  }
