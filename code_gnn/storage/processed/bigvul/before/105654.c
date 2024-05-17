TreeView::NodeDetails* TreeView::GetNodeDetailsByTreeItem(HTREEITEM tree_item) {
  DCHECK(tree_view_ && tree_item);
  TV_ITEM tv_item = {0};
  tv_item.hItem = tree_item;
  tv_item.mask = TVIF_PARAM;
  if (TreeView_GetItem(tree_view_, &tv_item))
    return GetNodeDetailsByID(static_cast<int>(tv_item.lParam));
  return NULL;
}
