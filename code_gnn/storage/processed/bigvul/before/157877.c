void WebContentsImpl::Undo() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->Undo();
  RecordAction(base::UserMetricsAction("Undo"));
}
