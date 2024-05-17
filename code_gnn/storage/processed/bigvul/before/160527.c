void WebContentsImpl::SetFocusedFrame(FrameTreeNode* node,
                                      SiteInstance* source) {
  frame_tree_.SetFocusedFrame(node, source);

  if (auto* inner_contents = node_.GetInnerWebContentsInFrame(node)) {
    if (GetFocusedWebContents() == this)
      inner_contents->SetAsFocusedWebContentsIfNecessary();
  } else if (node_.OuterContentsFrameTreeNode() &&
             node_.OuterContentsFrameTreeNode()
                     ->current_frame_host()
                     ->GetSiteInstance() == source) {
    if (GetFocusedWebContents() == GetOuterWebContents())
      SetAsFocusedWebContentsIfNecessary();
  } else if (!GetOuterWebContents()) {
    SetAsFocusedWebContentsIfNecessary();
  } else if (!GuestMode::IsCrossProcessFrameGuest(this) &&
             GetOuterWebContents()) {
    SetAsFocusedWebContentsIfNecessary();
   }
 }
