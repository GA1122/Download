void TreeView::TreeNodesRemoved(TreeModel* model,
                                TreeModelNode* parent,
                                int start,
                                int count) {
  DCHECK(parent && start >= 0 && count > 0);

  HTREEITEM tree_item;
  if (!root_shown_ && parent == model->GetRoot()) {
    tree_item = TreeView_GetRoot(tree_view_);
  } else {
    HTREEITEM parent_tree_item = GetTreeItemForNodeDuringMutation(parent);
    if (!parent_tree_item)
      return;

    tree_item = TreeView_GetChild(tree_view_, parent_tree_item);
  }

  for (int i = 0; i < (start + count - 1); ++i) {
    tree_item = TreeView_GetNextSibling(tree_view_, tree_item);
  }

  for (int i = count - 1; i >= 0; --i) {
    HTREEITEM previous = (start + i) > 0 ?
        TreeView_GetPrevSibling(tree_view_, tree_item) : NULL;
    RecursivelyDelete(GetNodeDetailsByTreeItem(tree_item));
    tree_item = previous;
  }
}
