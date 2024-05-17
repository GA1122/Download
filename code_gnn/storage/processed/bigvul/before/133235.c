void TaskQueue::PumpQueueLocked() {
  lock_.AssertAcquired();
  if (task_queue_manager_) {
    LazyNow lazy_now(task_queue_manager_);
    MoveReadyDelayedTasksToIncomingQueueLocked(&lazy_now);
  }
  while (!incoming_queue_.empty()) {
    work_queue_.push(incoming_queue_.front());
    incoming_queue_.pop();
  }
  if (!work_queue_.empty())
    task_queue_manager_->MaybePostDoWorkOnMainRunner();
}
