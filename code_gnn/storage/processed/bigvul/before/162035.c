void BrowserChildProcessHostImpl::OnProcessLaunchFailed(int error_code) {
  delegate_->OnProcessLaunchFailed(error_code);
  notify_child_disconnected_ = false;
  delete delegate_;   
}
