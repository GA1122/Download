void TaskQueue::ScheduleDelayedWorkLocked(LazyNow* lazy_now) {
  lock_.AssertAcquired();
  if (!delayed_task_queue_.empty()) {
    base::TimeTicks next_run_time = delayed_task_queue_.top().delayed_run_time;
    DCHECK_GT(next_run_time, lazy_now->Now());
    if (in_flight_kick_delayed_tasks_.find(next_run_time) ==
        in_flight_kick_delayed_tasks_.end()) {
      in_flight_kick_delayed_tasks_.insert(next_run_time);
      base::TimeDelta delay = next_run_time - lazy_now->Now();
      task_queue_manager_->PostDelayedTask(
          FROM_HERE,
          Bind(&TaskQueue::MoveReadyDelayedTasksToIncomingQueue, this), delay);
    }
  }
}
