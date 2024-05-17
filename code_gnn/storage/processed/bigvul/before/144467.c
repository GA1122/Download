void WebContentsImpl::Delete() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Delete(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("DeleteSelection"));
}
