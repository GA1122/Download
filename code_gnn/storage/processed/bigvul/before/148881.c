int RenderFrameHostManager::GetOpenerRoutingID(SiteInstance* instance) {
  if (!frame_tree_node_->opener())
    return MSG_ROUTING_NONE;

  return frame_tree_node_->opener()
      ->render_manager()
      ->GetRoutingIdForSiteInstance(instance);
}
