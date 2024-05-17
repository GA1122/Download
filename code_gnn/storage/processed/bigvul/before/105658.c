bool TreeView::IsExpanded(TreeModelNode* node) {
  TreeModelNode* parent = model_->GetParent(node);
  if (!parent)
    return true;
  if (!IsExpanded(parent))
    return false;
  NodeDetails* details = GetNodeDetails(node);
  return (TreeView_GetItemState(tree_view_, details->tree_item, TVIS_EXPANDED) &
          TVIS_EXPANDED) != 0;
}
