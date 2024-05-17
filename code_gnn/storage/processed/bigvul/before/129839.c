void BlockUntilStopped(WaitableEvent* task_start_event,
                       WaitableEvent* task_stop_event) {
  task_start_event->Signal();
  task_stop_event->Wait();
}
