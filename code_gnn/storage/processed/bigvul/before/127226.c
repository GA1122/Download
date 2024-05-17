void WorkerProcessLauncher::Core::OnObjectSignaled(HANDLE object) {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());
  DCHECK(process_watcher_.GetWatchedObject() == NULL);

  StopWorker();
}
