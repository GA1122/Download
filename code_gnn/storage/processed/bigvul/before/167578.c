bool RenderFrameHostManager::ReinitializeRenderFrame(
    RenderFrameHostImpl* render_frame_host) {
  DCHECK(!render_frame_host->IsRenderFrameLive());

  CreateOpenerProxies(render_frame_host->GetSiteInstance(), frame_tree_node_);

  if (!frame_tree_node_->parent()) {
    DCHECK(!GetRenderFrameProxyHost(render_frame_host->GetSiteInstance()));
    if (!InitRenderView(render_frame_host->render_view_host(), nullptr))
      return false;
  } else {
    if (!InitRenderFrame(render_frame_host))
      return false;

    RenderFrameProxyHost* proxy_to_parent = GetProxyToParent();
    if (proxy_to_parent) {
      const gfx::Size* size = render_frame_host->frame_size()
                                  ? &*render_frame_host->frame_size()
                                  : nullptr;
      GetProxyToParent()->SetChildRWHView(render_frame_host->GetView(), size);
    }
  }

  DCHECK(render_frame_host->IsRenderFrameLive());
  return true;
}
