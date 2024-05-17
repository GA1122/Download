void TreeView::TreeNodeChanged(TreeModel* model, TreeModelNode* node) {
  if (node_to_details_map_.find(node) == node_to_details_map_.end()) {
    return;
  }
  const NodeDetails* details = GetNodeDetails(node);
  TV_ITEM tv_item = {0};
  tv_item.mask = TVIF_TEXT;
  tv_item.hItem = details->tree_item;
  tv_item.pszText = LPSTR_TEXTCALLBACK;
  TreeView_SetItem(tree_view_, &tv_item);
}
