void TreeView::SetSelectedNode(TreeModelNode* node) {
  DCHECK(tree_view_);
  if (!node) {
    TreeView_SelectItem(tree_view_, NULL);
    return;
  }
  if (node != model_->GetRoot())
    Expand(model_->GetParent(node));
  if (!root_shown_ && node == model_->GetRoot()) {
    TreeView_SelectItem(tree_view_, NULL);
  } else {
    TreeView_SelectItem(tree_view_, GetNodeDetails(node)->tree_item);
  }
}
