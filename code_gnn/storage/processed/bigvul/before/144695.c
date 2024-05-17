void WebContentsImpl::Unselect() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Unselect(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Unselect"));
}
