bool SelectionEditor::NeedsUpdateVisibleSelectionInFlatTree() const {
  return cached_visible_selection_in_flat_tree_is_dirty_ ||
         style_version_for_flat_tree_ != GetDocument().StyleVersion();
}
