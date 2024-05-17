void WtsConsoleSessionProcessDriver::OnSessionAttached(uint32 session_id) {
  DCHECK(caller_task_runner_->BelongsToCurrentThread());

  if (stoppable_state() != Stoppable::kRunning) {
    return;
  }

  DCHECK(launcher_.get() == NULL);

  FilePath host_binary;
  if (!GetInstalledBinaryPath(kHostBinaryName, &host_binary)) {
    Stop();
    return;
  }

  scoped_ptr<WtsSessionProcessDelegate> delegate(
      new WtsSessionProcessDelegate(caller_task_runner_,
                                    io_task_runner_,
                                    host_binary,
                                    session_id,
                                    true,
                                    kDaemonIpcSecurityDescriptor));

  launcher_.reset(new WorkerProcessLauncher(
      caller_task_runner_, delegate.Pass(), this));
}
