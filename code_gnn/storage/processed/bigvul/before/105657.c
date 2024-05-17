HTREEITEM TreeView::GetTreeItemForNodeDuringMutation(TreeModelNode* node) {
  if (node_to_details_map_.find(node) == node_to_details_map_.end()) {
    return NULL;
  }
  if (!root_shown_ || node != model_->GetRoot()) {
    const NodeDetails* details = GetNodeDetails(node);
    if (!details->loaded_children)
      return NULL;
    return details->tree_item;
  }
  return TreeView_GetRoot(tree_view_);
}
