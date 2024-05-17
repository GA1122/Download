void WebContentsImpl::Paste() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Paste();
  RecordAction(base::UserMetricsAction("Paste"));
}
