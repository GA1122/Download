RenderViewHostImpl* RenderViewHostManager::Navigate(
    const NavigationEntryImpl& entry) {
  RenderViewHostImpl* dest_render_view_host =
      static_cast<RenderViewHostImpl*>(UpdateRendererStateForNavigate(entry));
  if (!dest_render_view_host)
    return NULL;   

  if (dest_render_view_host != render_view_host_ &&
      !render_view_host_->IsRenderViewLive()) {
    delegate_->CreateRenderViewForRenderManager(render_view_host_,
                                                MSG_ROUTING_NONE);
  }

  if (!dest_render_view_host->IsRenderViewLive()) {
    if (!InitRenderView(dest_render_view_host, MSG_ROUTING_NONE))
      return NULL;

    if (dest_render_view_host != render_view_host_ &&
        dest_render_view_host->GetView()) {
      dest_render_view_host->GetView()->Hide();
    } else {
      RenderViewHost* null_rvh = NULL;
      std::pair<RenderViewHost*, RenderViewHost*> details =
          std::make_pair(null_rvh, render_view_host_);
      NotificationService::current()->Notify(
          NOTIFICATION_RENDER_VIEW_HOST_CHANGED,
          Source<NavigationController>(
              &delegate_->GetControllerForRenderManager()),
          Details<std::pair<RenderViewHost*, RenderViewHost*> >(
              &details));
    }
  }

  return dest_render_view_host;
}
