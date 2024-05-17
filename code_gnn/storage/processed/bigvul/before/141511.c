void PaintLayerScrollableArea::UpdateAfterStyleChange(
    const ComputedStyle* old_style) {
  if (!OverflowRect().Size().IsZero()) {
    UpdateScrollableAreaSet();
  }

  Color old_background;
  if (old_style) {
    old_background =
        old_style->VisitedDependentColor(GetCSSPropertyBackgroundColor());
  }
  Color new_background = GetLayoutBox()->StyleRef().VisitedDependentColor(
      GetCSSPropertyBackgroundColor());

  if (new_background != old_background) {
    RecalculateScrollbarOverlayColorTheme(new_background);
  }

  bool needs_horizontal_scrollbar;
  bool needs_vertical_scrollbar;
  ComputeScrollbarExistence(needs_horizontal_scrollbar,
                            needs_vertical_scrollbar, kForbidAddingAutoBars);

  if (!HasScrollbar() && !needs_horizontal_scrollbar &&
      !needs_vertical_scrollbar)
    return;

  bool horizontal_scrollbar_changed =
      SetHasHorizontalScrollbar(needs_horizontal_scrollbar);
  bool vertical_scrollbar_changed =
      SetHasVerticalScrollbar(needs_vertical_scrollbar);

  if (GetLayoutBox()->IsLayoutBlock() &&
      (horizontal_scrollbar_changed || vertical_scrollbar_changed)) {
    ToLayoutBlock(GetLayoutBox())
        ->ScrollbarsChanged(horizontal_scrollbar_changed,
                            vertical_scrollbar_changed,
                            LayoutBlock::ScrollbarChangeContext::kStyleChange);
  }

  if (HasHorizontalScrollbar() && old_style &&
      old_style->OverflowX() == EOverflow::kScroll &&
      GetLayoutBox()->StyleRef().OverflowX() != EOverflow::kScroll) {
    HorizontalScrollbar()->SetEnabled(true);
  }

  if (HasVerticalScrollbar() && old_style &&
      old_style->OverflowY() == EOverflow::kScroll &&
      GetLayoutBox()->StyleRef().OverflowY() != EOverflow::kScroll) {
    VerticalScrollbar()->SetEnabled(true);
  }

  if (HorizontalScrollbar())
    HorizontalScrollbar()->StyleChanged();
  if (VerticalScrollbar())
    VerticalScrollbar()->StyleChanged();

  UpdateScrollCornerStyle();
  UpdateResizerAreaSet();
  UpdateResizerStyle(old_style);
}
