void TreeView::DeleteRootItems() {
  HTREEITEM root = TreeView_GetRoot(tree_view_);
  if (root) {
    if (root_shown_) {
      RecursivelyDelete(GetNodeDetailsByTreeItem(root));
    } else {
      do {
        RecursivelyDelete(GetNodeDetailsByTreeItem(root));
      } while ((root = TreeView_GetRoot(tree_view_)));
    }
  }
}
