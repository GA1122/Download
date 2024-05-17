void SelectionEditor::UpdateCachedVisibleSelectionIfNeeded() const {
  DCHECK_GE(GetDocument().Lifecycle().GetState(),
            DocumentLifecycle::kAfterPerformLayout);
  AssertSelectionValid();
  if (!NeedsUpdateVisibleSelection())
    return;
  style_version_for_dom_tree_ = GetDocument().StyleVersion();
  cached_visible_selection_in_dom_tree_is_dirty_ = false;
  cached_visible_selection_in_dom_tree_ = CreateVisibleSelection(selection_);
  if (!cached_visible_selection_in_dom_tree_.IsNone())
    return;
  style_version_for_flat_tree_ = GetDocument().StyleVersion();
  cached_visible_selection_in_flat_tree_is_dirty_ = false;
  cached_visible_selection_in_flat_tree_ = VisibleSelectionInFlatTree();
}
