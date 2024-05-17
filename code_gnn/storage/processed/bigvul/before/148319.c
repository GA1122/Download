void WebContentsImpl::CollapseSelection() {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->CollapseSelection();
}
