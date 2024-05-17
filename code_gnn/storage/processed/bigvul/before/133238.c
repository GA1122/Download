void TaskQueue::QueueAsValueInto(const base::DelayedTaskQueue& queue,
                                 base::trace_event::TracedValue* state) {
  base::DelayedTaskQueue queue_copy(queue);
  while (!queue_copy.empty()) {
    TaskAsValueInto(queue_copy.top(), state);
    queue_copy.pop();
  }
}
