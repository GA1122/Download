void DelegatedFrameHost::EvictDelegatedFrame() {
  if (!has_frame_)
    return;
  client_->DelegatedFrameHostGetLayer()->SetShowSolidColorContent();
  support_->EvictCurrentSurface();
  has_frame_ = false;
  resize_lock_.reset();
  frame_evictor_->DiscardedFrame();
  UpdateGutters();
}
