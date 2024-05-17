void TreeView::CreateRootItems() {
  DCHECK(model_);
  DCHECK(tree_view_);
  TreeModelNode* root = model_->GetRoot();
  if (root_shown_) {
    CreateItem(NULL, TVI_LAST, root);
  } else {
    for (int i = 0; i < model_->GetChildCount(root); ++i)
      CreateItem(NULL, TVI_LAST, model_->GetChild(root, i));
  }
}
