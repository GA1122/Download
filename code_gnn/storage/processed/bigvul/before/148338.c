void WebContentsImpl::Delete() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Delete();
  RecordAction(base::UserMetricsAction("DeleteSelection"));
}
