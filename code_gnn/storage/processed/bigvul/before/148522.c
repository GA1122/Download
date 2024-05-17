void WebContentsImpl::Redo() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;
  focused_frame->GetFrameInputHandler()->Redo();
  RecordAction(base::UserMetricsAction("Redo"));
}
