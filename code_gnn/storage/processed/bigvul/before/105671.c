void TreeView::TreeNodesAdded(TreeModel* model,
                              TreeModelNode* parent,
                              int start,
                              int count) {
  DCHECK(parent && start >= 0 && count > 0);
  if (node_to_details_map_.find(parent) == node_to_details_map_.end() &&
      (root_shown_ || parent != model_->GetRoot())) {
    return;
  }
  HTREEITEM parent_tree_item = NULL;
  if (root_shown_ || parent != model_->GetRoot()) {
    const NodeDetails* details = GetNodeDetails(parent);
    if (!details->loaded_children) {
      if (count == model_->GetChildCount(parent)) {
        TV_ITEM tv_item = {0};
        tv_item.mask = TVIF_CHILDREN;
        tv_item.cChildren = count;
        tv_item.hItem = details->tree_item;
        TreeView_SetItem(tree_view_, &tv_item);
      }

      return;
    }
    parent_tree_item = details->tree_item;
  }

  for (int i = 0; i < count; ++i) {
    if (i == 0 && start == 0) {
      CreateItem(parent_tree_item, TVI_FIRST, model_->GetChild(parent, 0));
    } else {
      TreeModelNode* previous_sibling = model_->GetChild(parent, i + start - 1);
      CreateItem(parent_tree_item,
                 GetNodeDetails(previous_sibling)->tree_item,
                 model_->GetChild(parent, i + start));
    }
  }
}
