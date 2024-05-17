void CompositorImpl::SetVisible(bool visible) {
  TRACE_EVENT1("cc", "CompositorImpl::SetVisible", "visible", visible);

  if (!visible) {
    DCHECK(host_->IsVisible());
    CompositorDependencies::Get().OnCompositorHidden(this);
    TearDownDisplayAndUnregisterRootFrameSink();
    host_->SetVisible(false);
    host_->ReleaseLayerTreeFrameSink();
    has_layer_tree_frame_sink_ = false;
    pending_frames_ = 0;
  } else {
    DCHECK(!host_->IsVisible());
    CompositorDependencies::Get().OnCompositorVisible(this);
    RegisterRootFrameSink();
    host_->SetVisible(true);
    has_submitted_frame_since_became_visible_ = false;
    if (layer_tree_frame_sink_request_pending_)
      HandlePendingLayerTreeFrameSinkRequest();
  }
}
