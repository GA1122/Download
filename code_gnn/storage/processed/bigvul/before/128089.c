void SynchronousCompositorImpl::SetIsActive(bool is_active) {
  TRACE_EVENT1("cc", "SynchronousCompositorImpl::SetIsActive", "is_active",
               is_active);
  is_active_ = is_active;
  UpdateNeedsBeginFrames();
}
