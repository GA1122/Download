void RenderFrameHostImpl::PendingDeletionCheckCompletedOnSubtree() {
  if (children_.empty()) {
    PendingDeletionCheckCompleted();
    return;
  }

  std::vector<RenderFrameHostImpl*> children_rfh;
  for (std::unique_ptr<FrameTreeNode>& child : children_)
    children_rfh.push_back(child->current_frame_host());

  for (RenderFrameHostImpl* child_rfh : children_rfh)
    child_rfh->PendingDeletionCheckCompletedOnSubtree();
}
