void DaemonProcess::CreateDesktopSession(int terminal_id) {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());

  if (IsTerminalIdKnown(terminal_id)) {
    LOG(ERROR) << "An invalid terminal ID. terminal_id=" << terminal_id;
    RestartNetworkProcess();
    DeleteAllDesktopSessions();
    return;
  }

  VLOG(1) << "Daemon: opened desktop session " << terminal_id;
  desktop_sessions_.push_back(
      DoCreateDesktopSession(terminal_id).release());
  next_terminal_id_ = std::max(next_terminal_id_, terminal_id + 1);
}
