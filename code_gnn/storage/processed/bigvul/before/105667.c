void TreeView::SetRootShown(bool root_shown) {
  if (root_shown_ == root_shown)
    return;
  root_shown_ = root_shown;
  if (!model_ || !tree_view_)
    return;
  DeleteRootItems();
  CreateRootItems();
}
