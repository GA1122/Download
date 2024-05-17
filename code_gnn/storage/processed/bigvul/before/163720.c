void WebContentsImpl::SetFocusedFrame(FrameTreeNode* node,
                                      SiteInstance* source) {
  SetAsFocusedWebContentsIfNecessary();

  frame_tree_.SetFocusedFrame(node, source);

  WebContentsImpl* inner_contents = node_.GetInnerWebContentsInFrame(node);

  WebContentsImpl* contents_to_focus = inner_contents ? inner_contents : this;
  contents_to_focus->SetAsFocusedWebContentsIfNecessary();
}
