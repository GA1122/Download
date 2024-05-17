void TaskQueue::QueueAsValueInto(const base::TaskQueue& queue,
                                 base::trace_event::TracedValue* state) {
  base::TaskQueue queue_copy(queue);
  while (!queue_copy.empty()) {
    TaskAsValueInto(queue_copy.front(), state);
    queue_copy.pop();
  }
}
