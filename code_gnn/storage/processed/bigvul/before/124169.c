void RenderViewHostManager::Stop() {
  render_view_host_->Stop();

  if (cross_navigation_pending_) {
    pending_render_view_host_->Send(
        new ViewMsg_Stop(pending_render_view_host_->GetRoutingID()));
  }
}
