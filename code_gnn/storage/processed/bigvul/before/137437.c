bool MessageLoop::DeferOrRunPendingTask(PendingTask pending_task) {
  if (pending_task.nestable || !run_loop_client_->IsNested()) {
    RunTask(&pending_task);
    return true;
  }

  deferred_non_nestable_work_queue_.push(std::move(pending_task));
  return false;
}
