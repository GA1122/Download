void WebContentsImpl::CollapseSelection() {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->CollapseSelection();
}
