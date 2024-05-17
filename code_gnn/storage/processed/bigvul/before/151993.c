bool RenderFrameHostImpl::IsRenderFrameLive() {
  bool is_live =
      GetProcess()->IsInitializedAndNotDead() && render_frame_created_;

  DCHECK(!is_live || render_view_host_->IsRenderViewLive());

  return is_live;
}
