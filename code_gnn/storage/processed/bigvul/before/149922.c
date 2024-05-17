BeginFrameArgs LayerTreeHostImpl::CurrentBeginFrameArgs() const {
  return current_begin_frame_tracker_.DangerousMethodCurrentOrLast();
}
