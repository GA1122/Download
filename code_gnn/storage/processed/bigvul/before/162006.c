bool BrowserChildProcessHostImpl::CanShutdown() {
  return delegate_->CanShutdown();
}
