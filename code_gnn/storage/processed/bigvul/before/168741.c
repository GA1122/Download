void RenderWidgetHostImpl::DidReceiveFirstFrameAfterNavigation() {
  DCHECK(enable_surface_synchronization_);
  if (!new_content_rendering_timeout_ ||
      !new_content_rendering_timeout_->IsRunning()) {
    return;
  }
   new_content_rendering_timeout_->Stop();
 }
