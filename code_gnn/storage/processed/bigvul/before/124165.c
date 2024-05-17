void RenderViewHostManager::ShouldClosePage(
    bool for_cross_site_transition,
    bool proceed,
    const base::TimeTicks& proceed_time) {
  if (for_cross_site_transition) {
    if (!cross_navigation_pending_)
      return;

    if (proceed) {
      if (pending_render_view_host_ &&
          pending_render_view_host_->are_navigations_suspended()) {
        pending_render_view_host_->SetNavigationsSuspended(false);
        if (!proceed_time.is_null()) {
          pending_render_view_host_->SetNavigationStartTime(proceed_time);
        }
      }
    } else {
      CancelPending();
      cross_navigation_pending_ = false;
    }
  } else {
    bool proceed_to_fire_unload;
    delegate_->BeforeUnloadFiredFromRenderManager(proceed, proceed_time,
                                                  &proceed_to_fire_unload);

    if (proceed_to_fire_unload) {
      render_view_host_->ClosePage();
    }
  }
}
