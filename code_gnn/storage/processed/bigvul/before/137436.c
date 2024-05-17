void MessageLoop::DecrementHighResTaskCountIfNeeded(
    const PendingTask& pending_task) {
  if (!pending_task.is_high_res)
    return;
  --pending_high_res_tasks_;
  DCHECK_GE(pending_high_res_tasks_, 0);
}
