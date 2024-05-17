void BrowserChildProcessHostImpl::SetHandle(base::ProcessHandle handle) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  data_.handle = handle;
}
