void TraceManyInstantEvents(int thread_id, int num_events,
                            WaitableEvent* task_complete_event) {
  for (int i = 0; i < num_events; i++) {
    TRACE_EVENT_INSTANT2("all", "multi thread event",
                         TRACE_EVENT_SCOPE_THREAD,
                         "thread", thread_id,
                         "event", i);
  }

  if (task_complete_event)
    task_complete_event->Signal();
}
