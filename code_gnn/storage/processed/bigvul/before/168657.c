void IndexedDBTransaction::ScheduleTask(blink::WebIDBTaskType type,
                                        Operation task) {
  DCHECK_NE(state_, COMMITTING);
  if (state_ == FINISHED)
    return;

  timeout_timer_.Stop();
  used_ = true;
  if (type == blink::kWebIDBTaskTypeNormal) {
    task_queue_.push(std::move(task));
    ++diagnostics_.tasks_scheduled;
  } else {
    preemptive_task_queue_.push(std::move(task));
  }
  RunTasksIfStarted();
}
