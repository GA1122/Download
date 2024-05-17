bool RenderFrameHostManager::CreateSpeculativeRenderFrameHost(
    SiteInstance* old_instance,
    SiteInstance* new_instance) {
  CHECK(new_instance);
  CHECK_NE(old_instance, new_instance);

  if (!new_instance->GetProcess()->Init())
    return false;

  CreateProxiesForNewRenderFrameHost(old_instance, new_instance);

  speculative_render_frame_host_ =
      CreateRenderFrame(new_instance, delegate_->IsHidden(), nullptr);

  if (speculative_render_frame_host_) {
    speculative_render_frame_host_->render_view_host()
        ->DispatchRenderViewCreated();
  }

  return !!speculative_render_frame_host_;
}
