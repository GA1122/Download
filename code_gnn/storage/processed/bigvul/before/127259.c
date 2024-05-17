bool WtsSessionProcessDelegate::LaunchProcess(
    IPC::Listener* delegate,
    base::win::ScopedHandle* process_exit_event_out) {
  if (!core_)
    return false;

  return core_->LaunchProcess(delegate, process_exit_event_out);
}
