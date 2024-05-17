void WorkerProcessLauncher::Core::LaunchWorker() {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());
  DCHECK(!ipc_enabled_);
  DCHECK(!launch_success_timer_->IsRunning());
  DCHECK(!launch_timer_->IsRunning());
  DCHECK(!process_exit_event_.IsValid());
  DCHECK(process_watcher_.GetWatchedObject() == NULL);

  if (launcher_delegate_->LaunchProcess(this, &process_exit_event_)) {
    if (process_watcher_.StartWatching(process_exit_event_, this)) {
      ipc_enabled_ = true;
      launch_success_timer_->Start(FROM_HERE, base::TimeDelta::FromSeconds(2),
                                   this, &Core::RecordSuccessfulLaunch);
      return;
    }

    launcher_delegate_->KillProcess(CONTROL_C_EXIT);
  }

  launch_backoff_.InformOfRequest(false);
  StopWorker();
}
