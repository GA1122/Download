RenderViewHostManager::~RenderViewHostManager() {
  if (pending_render_view_host_)
    CancelPending();

  RenderViewHostImpl* render_view_host = render_view_host_;
  render_view_host_ = NULL;
  if (render_view_host)
    render_view_host->Shutdown();

  for (RenderViewHostMap::iterator iter = swapped_out_hosts_.begin();
       iter != swapped_out_hosts_.end();
       ++iter) {
    iter->second->Shutdown();
  }
}
