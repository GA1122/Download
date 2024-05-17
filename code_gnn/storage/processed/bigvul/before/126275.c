int BrowserCommandController::GetLastBlockedCommand(
    WindowOpenDisposition* disposition) {
  if (disposition)
    *disposition = last_blocked_command_disposition_;
  return last_blocked_command_id_;
}
