void WebContentsImpl::Copy() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Copy();
  RecordAction(base::UserMetricsAction("Copy"));
}
