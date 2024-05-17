bool RenderFrameHostManager::ForInnerDelegate() {
  return delegate_->GetOuterDelegateFrameTreeNodeId() !=
      FrameTreeNode::kFrameTreeNodeInvalidId;
}
