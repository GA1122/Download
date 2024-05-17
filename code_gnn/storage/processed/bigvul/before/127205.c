void DesktopSessionWin::OnPermanentError() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  launcher_.reset();

  daemon_process()->CloseDesktopSession(id());
}
