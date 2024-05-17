void WebContentsImpl::Cut() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Cut(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Cut"));
}
