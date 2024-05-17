void WebContentsImpl::MoveRangeSelectionExtent(const gfx::Point& extent) {
  RenderFrameHost* focused_frame = GetFocusedFrame();
  if (!focused_frame)
    return;

  focused_frame->Send(new InputMsg_MoveRangeSelectionExtent(
      focused_frame->GetRoutingID(), extent));
}
