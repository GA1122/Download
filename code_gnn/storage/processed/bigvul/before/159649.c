void RenderFrameHostManager::OnBeforeUnloadACK(
    bool proceed,
    const base::TimeTicks& proceed_time) {
  bool proceed_to_fire_unload;
  delegate_->BeforeUnloadFiredFromRenderManager(proceed, proceed_time,
                                                &proceed_to_fire_unload);

  if (proceed_to_fire_unload) {
    if (speculative_render_frame_host_)
      CleanUpNavigation();

    render_frame_host_->render_view_host()->ClosePage();
  }
}
