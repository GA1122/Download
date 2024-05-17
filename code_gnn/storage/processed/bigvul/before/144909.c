RenderFrameHostImpl* RenderWidgetHostViewAura::GetFocusedFrame() {
  RenderViewHost* rvh = RenderViewHost::From(host_);
  if (!rvh)
    return nullptr;
  FrameTreeNode* focused_frame =
      rvh->GetDelegate()->GetFrameTree()->GetFocusedFrame();
  if (!focused_frame)
    return nullptr;

  return focused_frame->current_frame_host();
}
