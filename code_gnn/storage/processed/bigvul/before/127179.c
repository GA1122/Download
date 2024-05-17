void DaemonProcess::DoStop() {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());

  config_watcher_.reset();
  DeleteAllDesktopSessions();

  CompleteStopping();
}
