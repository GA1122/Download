void DelegatedFrameHost::SetNeedsBeginFrames(bool needs_begin_frames) {
  needs_begin_frame_ = needs_begin_frames;
  support_->SetNeedsBeginFrame(needs_begin_frames);
}
