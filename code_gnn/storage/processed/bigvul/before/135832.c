void SelectionEditor::MarkCacheDirty() {
  if (!cached_visible_selection_in_dom_tree_is_dirty_) {
    cached_visible_selection_in_dom_tree_ = VisibleSelection();
    cached_visible_selection_in_dom_tree_is_dirty_ = true;
  }
  if (!cached_visible_selection_in_flat_tree_is_dirty_) {
    cached_visible_selection_in_flat_tree_ = VisibleSelectionInFlatTree();
    cached_visible_selection_in_flat_tree_is_dirty_ = true;
  }
}
