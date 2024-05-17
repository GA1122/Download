bool PaintLayerScrollableArea::SetHasVerticalScrollbar(bool has_scrollbar) {
  if (FreezeScrollbarsScope::ScrollbarsAreFrozen())
    return false;

  if (GetLayoutBox()->GetDocument().IsVerticalScrollEnforced()) {
    return false;
  }

  if (has_scrollbar == HasVerticalScrollbar())
    return false;

  SetScrollbarNeedsPaintInvalidation(kVerticalScrollbar);

  scrollbar_manager_.SetHasVerticalScrollbar(has_scrollbar);

  UpdateScrollOrigin();

  if (HasHorizontalScrollbar())
    HorizontalScrollbar()->StyleChanged();
  if (HasVerticalScrollbar())
    VerticalScrollbar()->StyleChanged();

  SetScrollCornerNeedsPaintInvalidation();

  if (GetLayoutBox()->GetDocument().HasAnnotatedRegions())
    GetLayoutBox()->GetDocument().SetAnnotatedRegionsDirty(true);
  return true;
}
