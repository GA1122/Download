void BrowserChildProcessHostImpl::SetName(const base::string16& name) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  data_.name = name;
}
