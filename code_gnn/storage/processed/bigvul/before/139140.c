void RenderProcessHostImpl::ConnectionFilterController::DisableFilter() {
  base::AutoLock lock(lock_);
  if (filter_)
    filter_->Disable();
}
