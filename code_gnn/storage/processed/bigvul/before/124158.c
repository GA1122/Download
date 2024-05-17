void RenderViewHostManager::OnCrossSiteResponse(int new_render_process_host_id,
                                                int new_request_id) {
  if (!cross_navigation_pending_)
    return;
  DCHECK(pending_render_view_host_);

  render_view_host_->SwapOut(new_render_process_host_id, new_request_id);

  pending_render_view_host_->SetHasPendingCrossSiteRequest(false,
                                                           new_request_id);
}
