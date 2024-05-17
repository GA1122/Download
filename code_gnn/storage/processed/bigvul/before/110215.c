void NaClProcessHost::OnChannelConnected(int32 peer_pid) {
  if (!CommandLine::ForCurrentProcess()->GetSwitchValuePath(
          switches::kNaClGdb).empty()) {
    base::ProcessHandle process;
    DCHECK(process_->GetData().handle == base::kNullProcessHandle);
    if (base::OpenProcessHandleWithAccess(
            peer_pid,
            base::kProcessAccessDuplicateHandle |
            base::kProcessAccessQueryInformation |
            base::kProcessAccessWaitForTermination,
            &process)) {
      process_->SetHandle(process);
      if (!StartWithLaunchedProcess()) {
        delete this;
        return;
      }
    } else {
      DLOG(ERROR) << "Failed to get process handle";
    }
  }
}
