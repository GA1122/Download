void WebContentsImpl::Paste() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Paste(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Paste"));
}
