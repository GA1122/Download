void WebContentsImpl::SelectAll() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->SelectAll();
  RecordAction(base::UserMetricsAction("SelectAll"));
}
