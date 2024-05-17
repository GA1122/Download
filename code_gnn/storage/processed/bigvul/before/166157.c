void RenderFrameHostImpl::StartPendingDeletionOnSubtree() {
  DCHECK_EQ(UnloadState::InProgress, unload_state_);
  DCHECK(is_waiting_for_swapout_ack_);

  for (std::unique_ptr<FrameTreeNode>& child_frame : children_) {
    for (FrameTreeNode* node :
         frame_tree_node_->frame_tree()->SubtreeNodes(child_frame.get())) {
      RenderFrameHostImpl* child = node->current_frame_host();
      DCHECK_EQ(UnloadState::NotRun, child->unload_state_);

      RenderFrameHostImpl* local_ancestor = child;
      for (auto* rfh = child->parent_; rfh != parent_; rfh = rfh->parent_) {
        if (rfh->GetSiteInstance() == child->GetSiteInstance())
          local_ancestor = rfh;
      }

      local_ancestor->DeleteRenderFrame();
      if (local_ancestor != child) {
        child->unload_state_ =
            child->GetSuddenTerminationDisablerState(blink::kUnloadHandler)
                ? UnloadState::InProgress
                : UnloadState::Completed;
      }
    }
  }
}
