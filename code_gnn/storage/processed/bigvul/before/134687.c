 void SynchronousCompositorExternalBeginFrameSource::OnNeedsBeginFramesChange(
    bool needs_begin_frames) {
  DCHECK(CalledOnValidThread());
  if (compositor_)
    compositor_->OnNeedsBeginFramesChange(needs_begin_frames);
}
