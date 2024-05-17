RenderViewHostImpl* RenderViewHostManager::UpdateRendererStateForNavigate(
    const NavigationEntryImpl& entry) {
  if (cross_navigation_pending_) {
    if (pending_render_view_host_)
      CancelPending();
    cross_navigation_pending_ = false;
  }

  SiteInstance* curr_instance = render_view_host_->GetSiteInstance();

  SiteInstance* new_instance = curr_instance;
  const NavigationEntry* curr_entry =
      delegate_->GetLastCommittedNavigationEntryForRenderManager();
  bool is_guest_scheme = curr_instance->GetSiteURL().SchemeIs(
      chrome::kGuestScheme);
  bool force_swap = ShouldSwapProcessesForNavigation(curr_entry, &entry);
  if (!is_guest_scheme && (ShouldTransitionCrossSite() || force_swap))
    new_instance = GetSiteInstanceForEntry(entry, curr_instance);

  if (!is_guest_scheme && (new_instance != curr_instance || force_swap)) {
    DCHECK(!cross_navigation_pending_);

    pending_web_ui_.reset(
        delegate_->CreateWebUIForRenderManager(entry.GetURL()));
    pending_and_current_web_ui_.reset();

    int opener_route_id = MSG_ROUTING_NONE;
    if (new_instance->IsRelatedSiteInstance(curr_instance)) {
      opener_route_id =
          delegate_->CreateOpenerRenderViewsForRenderManager(new_instance);
    }

    int route_id = CreateRenderView(new_instance, opener_route_id, false);
    if (route_id == MSG_ROUTING_NONE)
      return NULL;

    if (!render_view_host_->IsRenderViewLive()) {
      if (!cross_navigation_pending_) {
        CommitPending();
        return render_view_host_;
      } else {
        NOTREACHED();
        return render_view_host_;
      }
    }

    render_view_host_->Send(
        new ViewMsg_Stop(render_view_host_->GetRoutingID()));

    DCHECK(!pending_render_view_host_->are_navigations_suspended());
    pending_render_view_host_->SetNavigationsSuspended(true);

    pending_render_view_host_->SetHasPendingCrossSiteRequest(true, -1);

    DCHECK(!cross_navigation_pending_);
    cross_navigation_pending_ = true;

    render_view_host_->FirePageBeforeUnload(true);

    return pending_render_view_host_;
  } else {
    if (ShouldReuseWebUI(curr_entry, &entry)) {
      pending_web_ui_.reset();
      pending_and_current_web_ui_ = web_ui_->AsWeakPtr();
    } else {
      pending_and_current_web_ui_.reset();
      pending_web_ui_.reset(
          delegate_->CreateWebUIForRenderManager(entry.GetURL()));
    }

    if (pending_web_ui() && render_view_host_->IsRenderViewLive())
      pending_web_ui()->GetController()->RenderViewReused(render_view_host_);

    if (entry.IsViewSourceMode()) {
      render_view_host_->Send(
          new ViewMsg_EnableViewSourceMode(render_view_host_->GetRoutingID()));
    }
  }

  DCHECK(!cross_navigation_pending_);
  return render_view_host_;
}
