void WebContentsImpl::Delete() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Delete();
  RecordAction(base::UserMetricsAction("DeleteSelection"));
}
