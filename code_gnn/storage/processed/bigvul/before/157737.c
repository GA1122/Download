bool WebContentsImpl::CreateRenderViewForInitialEmptyDocument() {
  return CreateRenderViewForRenderManager(
      GetRenderViewHost(), MSG_ROUTING_NONE, MSG_ROUTING_NONE,
      frame_tree_.root()->devtools_frame_token(),
      frame_tree_.root()->current_replication_state());
}