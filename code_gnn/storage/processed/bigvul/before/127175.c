void DaemonProcess::CloseDesktopSession(int terminal_id) {
  DCHECK(caller_task_runner()->BelongsToCurrentThread());

  if (!IsTerminalIdKnown(terminal_id)) {
    LOG(ERROR) << "An invalid terminal ID. terminal_id=" << terminal_id;
    RestartNetworkProcess();
    DeleteAllDesktopSessions();
    return;
  }

  DesktopSessionList::iterator i;
  for (i = desktop_sessions_.begin(); i != desktop_sessions_.end(); ++i) {
    if ((*i)->id() == terminal_id) {
      break;
    }
  }

  if (i == desktop_sessions_.end())
    return;

  delete *i;
  desktop_sessions_.erase(i);

  VLOG(1) << "Daemon: closed desktop session " << terminal_id;
  SendToNetwork(
      new ChromotingDaemonNetworkMsg_TerminalDisconnected(terminal_id));
}
