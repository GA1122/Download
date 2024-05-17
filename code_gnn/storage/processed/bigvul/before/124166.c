bool RenderViewHostManager::ShouldCloseTabOnUnresponsiveRenderer() {
  if (!cross_navigation_pending_)
    return true;

  int pending_request_id = pending_render_view_host_->GetPendingRequestId();
  if (pending_request_id == -1) {
    if (pending_render_view_host_->are_navigations_suspended())
      pending_render_view_host_->SetNavigationsSuspended(false);
  } else {
    ViewMsg_SwapOut_Params params;
    params.closing_process_id = render_view_host_->GetProcess()->GetID();
    params.closing_route_id = render_view_host_->GetRoutingID();
    params.new_render_process_host_id =
        pending_render_view_host_->GetProcess()->GetID();
    params.new_request_id = pending_request_id;
    current_host()->GetProcess()->SimulateSwapOutACK(params);
  }
  return false;
}
