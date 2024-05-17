void WebContentsImpl::Cut() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Cut();
  RecordAction(base::UserMetricsAction("Cut"));
}
