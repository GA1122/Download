void PageHandler::SetRenderer(int process_host_id,
                              RenderFrameHostImpl* frame_host) {
  if (host_ == frame_host)
    return;

  RenderWidgetHostImpl* widget_host =
      host_ ? host_->GetRenderWidgetHost() : nullptr;
  if (widget_host && observer_.IsObserving(widget_host))
    observer_.Remove(widget_host);

  host_ = frame_host;
  widget_host = host_ ? host_->GetRenderWidgetHost() : nullptr;

  if (widget_host)
    observer_.Add(widget_host);

  if (video_consumer_ && frame_host) {
    video_consumer_->SetFrameSinkId(
        frame_host->GetRenderWidgetHost()->GetFrameSinkId());
  }
}
