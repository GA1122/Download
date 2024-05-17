void Job::CheckIsOnWorkerThread() const {
  DCHECK(params_->worker_task_runner->BelongsToCurrentThread());
}
