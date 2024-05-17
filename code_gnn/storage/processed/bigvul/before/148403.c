RenderFrameHostImpl* WebContentsImpl::GetOpener() const {
  FrameTreeNode* opener_ftn = frame_tree_.root()->opener();
  return opener_ftn ? opener_ftn->current_frame_host() : nullptr;
}
