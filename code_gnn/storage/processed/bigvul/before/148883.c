RenderFrameHostManager::GetOuterRenderWidgetHostForKeyboardInput() {
  if (!ForInnerDelegate() || !frame_tree_node_->IsMainFrame())
    return nullptr;

  FrameTreeNode* outer_contents_frame_tree_node =
      FrameTreeNode::GloballyFindByID(
          delegate_->GetOuterDelegateFrameTreeNodeId());
  return outer_contents_frame_tree_node->parent()
      ->current_frame_host()
      ->render_view_host()
      ->GetWidget();
}
