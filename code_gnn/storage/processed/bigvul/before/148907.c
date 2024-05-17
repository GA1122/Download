void RenderFrameHostManager::RemoveOuterDelegateFrame() {
  FrameTreeNode* outer_delegate_frame_tree_node =
      FrameTreeNode::GloballyFindByID(
          delegate_->GetOuterDelegateFrameTreeNodeId());
  DCHECK(outer_delegate_frame_tree_node->parent());
  outer_delegate_frame_tree_node->frame_tree()->RemoveFrame(
      outer_delegate_frame_tree_node);
}
