void RenderViewHostManager::DidUpdateFrameTree(
    RenderViewHost* render_view_host) {
  if (render_view_host != current_host())
    return;

  RenderViewHostImpl* render_view_host_impl = static_cast<RenderViewHostImpl*>(
      render_view_host);

  for (RenderViewHostMap::iterator iter = swapped_out_hosts_.begin();
       iter != swapped_out_hosts_.end();
       ++iter) {
    DCHECK_NE(iter->second->GetSiteInstance(),
        current_host()->GetSiteInstance());

    if (iter->second != pending_render_view_host_) {
      iter->second->UpdateFrameTree(
          render_view_host_impl->GetProcess()->GetID(),
          render_view_host_impl->GetRoutingID(),
          render_view_host_impl->frame_tree());
    }
  }
}
