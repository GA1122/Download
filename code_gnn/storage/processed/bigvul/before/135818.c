SelectionEditor::ComputeVisibleSelectionInFlatTree() const {
  DCHECK_EQ(GetFrame()->GetDocument(), GetDocument());
  DCHECK_EQ(GetFrame(), GetDocument().GetFrame());
  UpdateCachedVisibleSelectionInFlatTreeIfNeeded();
  if (cached_visible_selection_in_flat_tree_.IsNone())
    return cached_visible_selection_in_flat_tree_;
  DCHECK_EQ(cached_visible_selection_in_flat_tree_.Base().GetDocument(),
            GetDocument());
  return cached_visible_selection_in_flat_tree_;
}
