void WebContentsImpl::Undo() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Undo(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Undo"));
}
