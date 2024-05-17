bool RenderViewHostManager::InitRenderView(RenderViewHost* render_view_host,
                                           int opener_route_id) {
  if (pending_web_ui())
    render_view_host->AllowBindings(pending_web_ui()->GetBindings());

  return delegate_->CreateRenderViewForRenderManager(render_view_host,
                                                     opener_route_id);
}
