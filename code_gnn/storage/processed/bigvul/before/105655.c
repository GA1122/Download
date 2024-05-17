TreeModelNode* TreeView::GetNodeForTreeItem(HTREEITEM tree_item) {
  NodeDetails* details = GetNodeDetailsByTreeItem(tree_item);
  return details ? details->node : NULL;
}
