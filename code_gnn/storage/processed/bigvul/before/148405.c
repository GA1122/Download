RenderFrameHostImpl* WebContentsImpl::GetOriginalOpener() const {
  FrameTreeNode* opener_ftn = frame_tree_.root()->original_opener();
  return opener_ftn ? opener_ftn->current_frame_host() : nullptr;
}
