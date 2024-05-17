void Compositor::OnNeedsExternalBeginFrames(bool needs_begin_frames) {
  DCHECK(external_begin_frames_enabled_);
  if (external_begin_frame_client_) {
    external_begin_frame_client_->OnNeedsExternalBeginFrames(
        needs_begin_frames);
  }
  needs_external_begin_frames_ = needs_begin_frames;
}
