WebContentsImpl* WebContentsImpl::GetOpener() const {
  FrameTreeNode* opener_ftn = frame_tree_.root()->opener();
  return opener_ftn ? FromFrameTreeNode(opener_ftn) : nullptr;
}
