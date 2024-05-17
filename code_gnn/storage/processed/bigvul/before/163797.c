void DelegatedFrameHost::WasShown(const ui::LatencyInfo& latency_info) {
  frame_evictor_->SetVisible(true);

  if (!has_frame_ && !released_front_lock_.get()) {
    if (compositor_)
      released_front_lock_ = compositor_->GetCompositorLock(nullptr);
  }

  if (compositor_) {
    compositor_->SetLatencyInfo(latency_info);
  }
}
