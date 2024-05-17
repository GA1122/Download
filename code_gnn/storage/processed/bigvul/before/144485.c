void WebContentsImpl::DidStartLoading(FrameTreeNode* frame_tree_node,
                                      bool to_different_document) {
  SetIsLoading(true, to_different_document, nullptr);

  BrowserAccessibilityManager* manager =
      frame_tree_node->current_frame_host()->browser_accessibility_manager();
  if (manager)
    manager->UserIsNavigatingAway();
}
