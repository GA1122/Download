void InterstitialPageImpl::SetFocusedFrame(FrameTreeNode* node,
                                           SiteInstance* source) {
  frame_tree_->SetFocusedFrame(node, source);

  if (web_contents_) {
    static_cast<WebContentsImpl*>(web_contents_)
        ->SetAsFocusedWebContentsIfNecessary();
  }
}
