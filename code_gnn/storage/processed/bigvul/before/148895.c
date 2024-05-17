void RenderFrameHostManager::OnBeforeUnloadACK(
    bool for_cross_site_transition,
    bool proceed,
    const base::TimeTicks& proceed_time) {
  if (for_cross_site_transition) {
    DCHECK(!IsBrowserSideNavigationEnabled());
    if (!pending_render_frame_host_)
      return;

    if (proceed) {
      if (pending_render_frame_host_ &&
          pending_render_frame_host_->are_navigations_suspended()) {
        pending_render_frame_host_->SetNavigationsSuspended(false,
                                                            proceed_time);
      }
    } else {
      CancelPending();
    }
  } else {
    bool proceed_to_fire_unload;
    delegate_->BeforeUnloadFiredFromRenderManager(proceed, proceed_time,
                                                  &proceed_to_fire_unload);

    if (proceed_to_fire_unload) {
      if (pending_render_frame_host_) {
        CancelPending();
      }

      if (IsBrowserSideNavigationEnabled() && speculative_render_frame_host_)
        CleanUpNavigation();

      render_frame_host_->render_view_host()->ClosePage();
    }
  }
}
