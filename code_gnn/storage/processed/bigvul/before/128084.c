void SynchronousCompositorImpl::OnNeedsBeginFramesChange(
    bool needs_begin_frames) {
  renderer_needs_begin_frames_ = needs_begin_frames;
  UpdateNeedsBeginFrames();
}
