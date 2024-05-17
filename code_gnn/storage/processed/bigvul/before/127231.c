void WorkerProcessLauncher::Core::Stop() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  if (!stopping_) {
    stopping_ = true;
    worker_delegate_ = NULL;
    StopWorker();
  }
}
