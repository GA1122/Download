void TaskQueue::TraceQueueSize(bool is_locked) const {
  bool is_tracing;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(disabled_by_default_tracing_category_,
                                     &is_tracing);
  if (!is_tracing || !name_)
    return;
  if (!is_locked)
    lock_.Acquire();
  else
    lock_.AssertAcquired();
  TRACE_COUNTER1(
      disabled_by_default_tracing_category_, name_,
      incoming_queue_.size() + work_queue_.size() + delayed_task_queue_.size());
  if (!is_locked)
    lock_.Release();
}
