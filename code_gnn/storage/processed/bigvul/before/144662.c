void WebContentsImpl::SelectAll() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_SelectAll(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("SelectAll"));
}
