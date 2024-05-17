void SchedulerHelper::EnableQueue(
    size_t queue_index,
    PrioritizingTaskQueueSelector::QueuePriority priority) {
  CheckOnValidThread();
  task_queue_selector_->EnableQueue(queue_index, priority);
}
