void RenderFrameDevToolsAgentHost::DidReceiveCompositorFrame() {
  const viz::CompositorFrameMetadata& metadata =
      RenderWidgetHostImpl::From(
          web_contents()->GetRenderViewHost()->GetWidget())
          ->last_frame_metadata();
  for (auto* page : protocol::PageHandler::ForAgentHost(this))
    page->OnSwapCompositorFrame(metadata.Clone());

  if (!frame_trace_recorder_)
    return;
  bool did_initiate_recording = false;
  for (auto* tracing : protocol::TracingHandler::ForAgentHost(this))
    did_initiate_recording |= tracing->did_initiate_recording();
  if (did_initiate_recording)
    frame_trace_recorder_->OnSwapCompositorFrame(frame_host_, metadata);
}
