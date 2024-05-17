void WebContentsImpl::MoveRangeSelectionExtent(const gfx::Point& extent) {
  RenderFrameHostImpl* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->GetFrameInputHandler()->MoveRangeSelectionExtent(extent);
}
