void DelegatedFrameHost::ResetCompositorFrameSinkSupport() {
  if (!support_)
    return;
  if (compositor_)
    compositor_->RemoveFrameSink(frame_sink_id_);
  support_.reset();
}
