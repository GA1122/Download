void TreeView::ExpandAll(TreeModelNode* node) {
  DCHECK(node);
  if (node != model_->GetRoot() || root_shown_)
    TreeView_Expand(tree_view_, GetNodeDetails(node)->tree_item, TVE_EXPAND);
  for (int i = model_->GetChildCount(node) - 1; i >= 0; --i) {
    TreeModelNode* child = model_->GetChild(node, i);
    ExpandAll(child);
  }
}
