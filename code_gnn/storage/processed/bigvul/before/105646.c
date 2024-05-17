void TreeView::Expand(TreeModelNode* node) {
  DCHECK(model_ && node);
  if (!root_shown_ && model_->GetRoot() == node) {
    return;
  }
  TreeModelNode* parent = model_->GetParent(node);
  if (parent) {
    Expand(parent);
  }
  TreeView_Expand(tree_view_, GetNodeDetails(node)->tree_item, TVE_EXPAND);
}
