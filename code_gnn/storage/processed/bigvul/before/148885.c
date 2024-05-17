RenderFrameProxyHost* RenderFrameHostManager::GetProxyToParent() {
  if (frame_tree_node_->IsMainFrame())
    return nullptr;

  return GetRenderFrameProxyHost(frame_tree_node_->parent()
                                     ->render_manager()
                                     ->current_frame_host()
                                     ->GetSiteInstance());
}
