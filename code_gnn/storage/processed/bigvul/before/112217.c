 void QuotaTask::CallCompleted() {
  DCHECK(original_task_runner_->BelongsToCurrentThread());
  if (observer_) {
    observer_->UnregisterTask(this);
    Completed();
  }
}
