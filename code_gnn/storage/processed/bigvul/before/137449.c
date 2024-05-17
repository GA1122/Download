bool MessageLoop::ProcessNextDelayedNonNestableTask() {
  if (run_loop_client_->IsNested())
    return false;

  while (!deferred_non_nestable_work_queue_.empty()) {
    PendingTask pending_task =
        std::move(deferred_non_nestable_work_queue_.front());
    deferred_non_nestable_work_queue_.pop();

    if (!pending_task.task.IsCancelled()) {
      RunTask(&pending_task);
      return true;
    }

#if defined(OS_WIN)
    DecrementHighResTaskCountIfNeeded(pending_task);
#endif
  }

  return false;
 }
