const base::Process& BrowserChildProcessHostImpl::GetProcess() const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(child_process_.get())
      << "Requesting a child process handle before launching.";
  DCHECK(child_process_->GetProcess().IsValid())
      << "Requesting a child process handle before launch has completed OK.";
  return child_process_->GetProcess();
}
