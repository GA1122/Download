void SelectionEditor::UpdateCachedVisibleSelectionInFlatTreeIfNeeded() const {
  DCHECK_GE(GetDocument().Lifecycle().GetState(),
            DocumentLifecycle::kAfterPerformLayout);
  AssertSelectionValid();
  if (!NeedsUpdateVisibleSelectionInFlatTree())
    return;
  style_version_for_flat_tree_ = GetDocument().StyleVersion();
  cached_visible_selection_in_flat_tree_is_dirty_ = false;
  SelectionInFlatTree::Builder builder;
  const PositionInFlatTree& base = ToPositionInFlatTree(selection_.Base());
  const PositionInFlatTree& extent = ToPositionInFlatTree(selection_.Extent());
  if (base.IsNotNull() && extent.IsNotNull())
    builder.SetBaseAndExtent(base, extent);
  else if (base.IsNotNull())
    builder.Collapse(base);
  else if (extent.IsNotNull())
    builder.Collapse(extent);
  builder.SetAffinity(selection_.Affinity())
      .SetIsDirectional(selection_.IsDirectional());
  cached_visible_selection_in_flat_tree_ =
      CreateVisibleSelection(builder.Build());
  if (!cached_visible_selection_in_flat_tree_.IsNone())
    return;
  style_version_for_dom_tree_ = GetDocument().StyleVersion();
  cached_visible_selection_in_dom_tree_is_dirty_ = false;
  cached_visible_selection_in_dom_tree_ = VisibleSelection();
}
