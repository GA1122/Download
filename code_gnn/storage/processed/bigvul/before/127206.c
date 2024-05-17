void DesktopSessionWin::OnSessionAttached(uint32 session_id) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK(launcher_.get() == NULL);

  if (desktop_binary_.empty()) {
    FilePath dir_path;
    if (!PathService::Get(base::DIR_EXE, &dir_path)) {
      LOG(ERROR) << "Failed to get the executable file name.";
      OnPermanentError();
      return;
    }
    desktop_binary_ = dir_path.Append(kDesktopBinaryName);
  }

  scoped_ptr<WtsSessionProcessDelegate> delegate(
      new WtsSessionProcessDelegate(main_task_runner_, io_task_runner_,
                                    desktop_binary_, session_id,
                                    true, kDaemonIpcSecurityDescriptor));

  launcher_.reset(new WorkerProcessLauncher(
      main_task_runner_, delegate.Pass(), this));
}
