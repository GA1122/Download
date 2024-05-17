void WebContentsImpl::Copy() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_Copy(focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("Copy"));
}
