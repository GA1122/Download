void RenderViewHostManager::DidNavigateMainFrame(
    RenderViewHost* render_view_host) {
  if (!cross_navigation_pending_) {
    DCHECK(!pending_render_view_host_);

    DCHECK(render_view_host == render_view_host_);

    if (pending_web_ui())
      CommitPending();
    return;
  }

  if (render_view_host == pending_render_view_host_) {
    if (pending_render_view_host_->GetPendingRequestId() == -1) {
      OnCrossSiteResponse(pending_render_view_host_->GetProcess()->GetID(),
                          pending_render_view_host_->GetRoutingID());
    }

    CommitPending();
    cross_navigation_pending_ = false;
  } else if (render_view_host == render_view_host_) {
    CancelPending();
    cross_navigation_pending_ = false;
  } else {
    DCHECK(false);
  }
}
