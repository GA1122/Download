void SelectionEditor::ClearVisibleSelection() {
  selection_ = SelectionInDOMTree();
  cached_visible_selection_in_dom_tree_ = VisibleSelection();
  cached_visible_selection_in_flat_tree_ = VisibleSelectionInFlatTree();
  cached_visible_selection_in_dom_tree_is_dirty_ = false;
  cached_visible_selection_in_flat_tree_is_dirty_ = false;
  if (!ShouldAlwaysUseDirectionalSelection())
    return;
  selection_.is_directional_ = true;
}
