bool BrowserChildProcessHostImpl::IsProcessLaunched() const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  return child_process_.get() && child_process_->GetProcess().IsValid();
}
