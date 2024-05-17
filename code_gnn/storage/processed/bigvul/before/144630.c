void WebContentsImpl::Redo() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;
  focused_frame->Send(new InputMsg_Redo(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Redo"));
}
