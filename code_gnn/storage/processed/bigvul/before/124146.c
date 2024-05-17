void RenderViewHostManager::CancelPending() {
  RenderViewHostImpl* pending_render_view_host = pending_render_view_host_;
  pending_render_view_host_ = NULL;

  RenderViewDevToolsAgentHost::OnCancelPendingNavigation(
      pending_render_view_host,
      render_view_host_);

  pending_render_view_host->GetProcess()->RemovePendingView();

  if (IsSwappedOut(pending_render_view_host)) {
    pending_render_view_host->CancelSuspendedNavigations();

    pending_render_view_host->SwapOut(-1, -1);
  } else {
    pending_render_view_host->Shutdown();
  }

  pending_web_ui_.reset();
  pending_and_current_web_ui_.reset();
}
