void DelegatedFrameHost::MaybeCreateResizeLock() {
  DCHECK(!resize_lock_);

  if (!compositor_)
    return;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableResizeLock))
    return;

  if (!has_frame_)
    return;

  if (!client_->DelegatedFrameCanCreateResizeLock())
    return;

  gfx::Size desired_size = client_->DelegatedFrameHostDesiredSizeInDIP();
  if (desired_size.IsEmpty())
    return;
  if (desired_size == current_frame_size_in_dip_)
    return;

  resize_lock_ = client_->DelegatedFrameHostCreateResizeLock();
  bool locked = resize_lock_->Lock();
  DCHECK(locked);
}
