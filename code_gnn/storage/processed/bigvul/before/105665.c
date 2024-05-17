void TreeView::SetEditable(bool editable) {
  if (editable == editable_)
    return;
  editable_ = editable;
  if (!tree_view_)
    return;
  LONG_PTR style = GetWindowLongPtr(tree_view_, GWL_STYLE);
  style &= ~TVS_EDITLABELS;
  SetWindowLongPtr(tree_view_, GWL_STYLE, style);
}
