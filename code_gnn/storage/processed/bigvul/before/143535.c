void CompositorImpl::SetVSyncPaused(bool paused) {
  if (!enable_viz_)
    return;

  if (vsync_paused_ == paused)
    return;

  vsync_paused_ = paused;
  if (display_private_)
    display_private_->SetVSyncPaused(paused);
}
