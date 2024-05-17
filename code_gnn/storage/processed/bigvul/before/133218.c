TaskQueueManager::QueueState TaskQueue::GetQueueState() const {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  if (!work_queue_.empty())
    return TaskQueueManager::QueueState::HAS_WORK;

  {
    base::AutoLock lock(lock_);
    if (incoming_queue_.empty()) {
      return TaskQueueManager::QueueState::EMPTY;
    } else {
      return TaskQueueManager::QueueState::NEEDS_PUMPING;
    }
  }
}
