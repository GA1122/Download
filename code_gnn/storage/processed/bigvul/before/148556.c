void WebContentsImpl::SetFocusedFrame(FrameTreeNode* node,
                                      SiteInstance* source) {
  SetAsFocusedWebContentsIfNecessary();
  frame_tree_.SetFocusedFrame(node, source);
}
