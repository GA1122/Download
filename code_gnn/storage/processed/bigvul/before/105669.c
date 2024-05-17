void TreeView::StartEditing(TreeModelNode* node) {
  DCHECK(node && tree_view_);
  CancelEdit();
  if (model_->GetParent(node))
    Expand(model_->GetParent(node));
  const NodeDetails* details = GetNodeDetails(node);
  SetFocus(tree_view_);
  SetSelectedNode(node);
  TreeView_EditLabel(tree_view_, details->tree_item);
}
