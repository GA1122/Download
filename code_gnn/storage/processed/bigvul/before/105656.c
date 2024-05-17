HTREEITEM TreeView::GetTreeItemForNode(TreeModelNode* node) {
  NodeDetails* details = GetNodeDetails(node);
  return details ? details->tree_item : NULL;
}
