void DelegatedFrameHost::OnCompositingLockStateChanged(
    ui::Compositor* compositor) {
  if (resize_lock_ && resize_lock_->timed_out()) {
    resize_lock_.reset();
    create_resize_lock_after_commit_ = true;
    allow_one_renderer_frame_during_resize_lock_ = true;
  }
}
