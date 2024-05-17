void DelegatedFrameHost::WasResized() {
  if (client_->DelegatedFrameHostDesiredSizeInDIP() !=
          current_frame_size_in_dip_ &&
      !client_->DelegatedFrameHostIsVisible())
    EvictDelegatedFrame();
  if (!resize_lock_ && !create_resize_lock_after_commit_)
    MaybeCreateResizeLock();
  UpdateGutters();
}
