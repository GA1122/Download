void QuotaTask::Abort() {
  DCHECK(original_task_runner_->BelongsToCurrentThread());
  observer_ = NULL;
  Aborted();
 }
