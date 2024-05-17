void RenderWidgetHostViewAura::OnBeginFrame(
    const cc::BeginFrameArgs& args) {
  needs_flush_input_ = false;
  host_->FlushInput();
  UpdateNeedsBeginFramesInternal();
  host_->Send(new ViewMsg_BeginFrame(host_->GetRoutingID(), args));
}
