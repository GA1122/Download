void RenderFrameHostImpl::OnAbortNavigation() {
  if (!IsBrowserSideNavigationEnabled()) {
    NOTREACHED();
    return;
  }
  if (!is_active())
    return;
  frame_tree_node()->navigator()->OnAbortNavigation(frame_tree_node());
}
