void WebContentsImpl::PasteAndMatchStyle() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_PasteAndMatchStyle(
      focused_frame->GetRoutingID()));
  RecordAction(base::UserMetricsAction("PasteAndMatchStyle"));
}
