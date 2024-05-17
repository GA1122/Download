bool TestRenderViewHost::CreateRenderView(
    int opener_frame_route_id,
    int proxy_route_id,
    const FrameReplicationState& replicated_frame_state,
    bool window_was_created_with_opener) {
  DCHECK(!IsRenderViewLive());
  GetWidget()->set_renderer_initialized(true);
  DCHECK(IsRenderViewLive());
  opener_frame_route_id_ = opener_frame_route_id;
  RenderFrameHost* main_frame = GetMainFrame();
  if (main_frame)
    static_cast<RenderFrameHostImpl*>(main_frame)->SetRenderFrameCreated(true);

  return true;
}
