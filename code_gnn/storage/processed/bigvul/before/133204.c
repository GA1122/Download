void SchedulerHelper::SetQueuePriority(
    size_t queue_index,
    PrioritizingTaskQueueSelector::QueuePriority priority) {
  CheckOnValidThread();
  return task_queue_selector_->SetQueuePriority(queue_index, priority);
}
