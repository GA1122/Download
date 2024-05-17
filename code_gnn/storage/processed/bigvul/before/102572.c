base::TerminationStatus NaClProcessHost::GetChildTerminationStatus(
    int* exit_code) {
  if (running_on_wow64_)
    return base::GetTerminationStatus(handle(), exit_code);
  return BrowserChildProcessHost::GetChildTerminationStatus(exit_code);
}
