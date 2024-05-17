void WorkerProcessLauncher::Core::RecordSuccessfulLaunch() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  launch_backoff_.InformOfRequest(true);
}
