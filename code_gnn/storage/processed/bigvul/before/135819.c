void SelectionEditor::ContextDestroyed(Document*) {
  Dispose();
  style_version_for_dom_tree_ = static_cast<uint64_t>(-1);
  style_version_for_flat_tree_ = static_cast<uint64_t>(-1);
  selection_ = SelectionInDOMTree();
  cached_visible_selection_in_dom_tree_ = VisibleSelection();
  cached_visible_selection_in_flat_tree_ = VisibleSelectionInFlatTree();
  cached_visible_selection_in_dom_tree_is_dirty_ = false;
  cached_visible_selection_in_flat_tree_is_dirty_ = false;
}
