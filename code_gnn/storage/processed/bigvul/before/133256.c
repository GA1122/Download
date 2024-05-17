bool TaskQueue::UpdateWorkQueue(LazyNow* lazy_now,
                                bool should_trigger_wakeup,
                                const base::PendingTask* previous_task) {
  if (!work_queue_.empty())
    return true;

  {
    base::AutoLock lock(lock_);
    if (!ShouldAutoPumpQueueLocked(should_trigger_wakeup, previous_task))
      return false;
    MoveReadyDelayedTasksToIncomingQueueLocked(lazy_now);
    work_queue_.Swap(&incoming_queue_);
    TraceQueueSize(true);
    return true;
  }
}
