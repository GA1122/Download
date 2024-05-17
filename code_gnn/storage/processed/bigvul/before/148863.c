void RenderFrameHostManager::CreatePendingRenderFrameHost(
    SiteInstance* old_instance,
    SiteInstance* new_instance) {
  if (pending_render_frame_host_)
    CancelPending();

  if (!new_instance->GetProcess()->Init())
    return;

  CreateProxiesForNewRenderFrameHost(old_instance, new_instance);

  pending_render_frame_host_ =
      CreateRenderFrame(new_instance, delegate_->IsHidden(), nullptr);
}
