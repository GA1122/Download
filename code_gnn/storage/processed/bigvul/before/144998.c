void RenderWidgetHostViewAura::UpdateNeedsBeginFramesInternal() {
  if (!delegated_frame_host_)
    return;
  delegated_frame_host_->SetNeedsBeginFrames(needs_begin_frames_ ||
                                             needs_flush_input_);
}
