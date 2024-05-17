bool DesktopSessionWin::OnMessageReceived(const IPC::Message& message) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  return false;
}
