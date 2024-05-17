ChildProcessHost* BrowserChildProcessHostImpl::GetHost() const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return child_process_host_.get();
}
