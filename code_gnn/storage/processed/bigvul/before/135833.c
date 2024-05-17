bool SelectionEditor::NeedsUpdateVisibleSelection() const {
  return cached_visible_selection_in_dom_tree_is_dirty_ ||
         style_version_for_dom_tree_ != GetDocument().StyleVersion();
}
