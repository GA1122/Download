void MessageLoop::AddToDelayedWorkQueue(PendingTask pending_task) {
  delayed_work_queue_.push(std::move(pending_task));
}
