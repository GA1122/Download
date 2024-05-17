void TreeView::CancelEdit() {
  DCHECK(tree_view_);
  TreeView_EndEditLabelNow(tree_view_, TRUE);
}
