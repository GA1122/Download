void WorkerProcessLauncher::Core::Start() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());
  DCHECK(!stopping_);

  LaunchWorker();
}
