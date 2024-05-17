const ChildProcessData& BrowserChildProcessHostImpl::GetData() const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return data_;
}
