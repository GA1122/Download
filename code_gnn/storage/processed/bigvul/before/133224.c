void TaskQueue::MoveReadyDelayedTasksToIncomingQueueLocked(LazyNow* lazy_now) {
  lock_.AssertAcquired();
  while (!delayed_task_queue_.empty() &&
         delayed_task_queue_.top().delayed_run_time <= lazy_now->Now()) {
    in_flight_kick_delayed_tasks_.erase(
        delayed_task_queue_.top().delayed_run_time);
    EnqueueTaskLocked(delayed_task_queue_.top());
    delayed_task_queue_.pop();
  }
  TraceQueueSize(true);
  ScheduleDelayedWorkLocked(lazy_now);
}
