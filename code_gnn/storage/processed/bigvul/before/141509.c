void PaintLayerScrollableArea::UpdateAfterLayout() {
  bool scrollbars_are_frozen =
      (in_overflow_relayout_ && !allow_second_overflow_relayout_) ||
      FreezeScrollbarsScope::ScrollbarsAreFrozen();
  allow_second_overflow_relayout_ = false;

  if (NeedsScrollbarReconstruction()) {
    SetHasHorizontalScrollbar(false);
    SetHasVerticalScrollbar(false);
  }

  UpdateScrollDimensions();

  bool had_horizontal_scrollbar = HasHorizontalScrollbar();
  bool had_vertical_scrollbar = HasVerticalScrollbar();

  bool needs_horizontal_scrollbar;
  bool needs_vertical_scrollbar;
  ComputeScrollbarExistence(needs_horizontal_scrollbar,
                            needs_vertical_scrollbar);

  if (!in_overflow_relayout_ && !scrollbars_are_frozen &&
      TryRemovingAutoScrollbars(needs_horizontal_scrollbar,
                                needs_vertical_scrollbar)) {
    needs_horizontal_scrollbar = needs_vertical_scrollbar = false;
    allow_second_overflow_relayout_ = true;
  }

  bool horizontal_scrollbar_should_change =
      needs_horizontal_scrollbar != had_horizontal_scrollbar;
  bool vertical_scrollbar_should_change =
      needs_vertical_scrollbar != had_vertical_scrollbar;

  bool scrollbars_will_change =
      !scrollbars_are_frozen &&
      (horizontal_scrollbar_should_change || vertical_scrollbar_should_change);
  if (scrollbars_will_change) {
    SetHasHorizontalScrollbar(needs_horizontal_scrollbar);
    SetHasVerticalScrollbar(needs_vertical_scrollbar);

    if (LocalFrameView* frame_view = GetLayoutBox()->GetFrameView()) {
      if (this == frame_view->LayoutViewport()) {
        GetLayoutBox()
            ->GetFrame()
            ->GetPage()
            ->GetVisualViewport()
            .SetNeedsPaintPropertyUpdate();
      }
    }

    if (HasScrollbar())
      UpdateScrollCornerStyle();

    Layer()->UpdateSelfPaintingLayer();

    if (GetLayoutBox()->GetDocument().HasAnnotatedRegions())
      GetLayoutBox()->GetDocument().SetAnnotatedRegionsDirty(true);

    if (((horizontal_scrollbar_should_change &&
          GetLayoutBox()->StyleRef().OverflowX() != EOverflow::kOverlay) ||
         (vertical_scrollbar_should_change &&
          GetLayoutBox()->StyleRef().OverflowY() != EOverflow::kOverlay))) {
      if ((vertical_scrollbar_should_change &&
           GetLayoutBox()->IsHorizontalWritingMode()) ||
          (horizontal_scrollbar_should_change &&
           !GetLayoutBox()->IsHorizontalWritingMode())) {
        GetLayoutBox()->SetPreferredLogicalWidthsDirty();
      }
      if (IsManagedByLayoutNG(*GetLayoutBox())) {
        UpdateScrollDimensions();
      } else {
        if (PreventRelayoutScope::RelayoutIsPrevented()) {
          GetLayoutBox()->UpdateLogicalWidth();
          PreventRelayoutScope::SetBoxNeedsLayout(
              *this, had_horizontal_scrollbar, had_vertical_scrollbar);
        } else {
          in_overflow_relayout_ = true;
          SubtreeLayoutScope layout_scope(*GetLayoutBox());
          layout_scope.SetNeedsLayout(
              GetLayoutBox(), layout_invalidation_reason::kScrollbarChanged);
          if (GetLayoutBox()->IsLayoutBlock()) {
            LayoutBlock* block = ToLayoutBlock(GetLayoutBox());
            block->ScrollbarsChanged(horizontal_scrollbar_should_change,
                                     vertical_scrollbar_should_change);
            block->UpdateBlockLayout(true);
          } else {
            GetLayoutBox()->UpdateLayout();
          }
          in_overflow_relayout_ = false;
          scrollbar_manager_.DestroyDetachedScrollbars();
        }
        LayoutObject* parent = GetLayoutBox()->Parent();
        if (parent && parent->IsFlexibleBox()) {
          ToLayoutFlexibleBox(parent)->ClearCachedMainSizeForChild(
              *GetLayoutBox());
        }
      }
    }
  }

  {
    DisableCompositingQueryAsserts disabler;

    UpdateScrollbarEnabledState();

    UpdateScrollbarProportions();
  }

  if (!scrollbars_are_frozen && HasOverlayScrollbars()) {
    if (!ScrollSize(kHorizontalScrollbar))
      SetHasHorizontalScrollbar(false);
    if (!ScrollSize(kVerticalScrollbar))
      SetHasVerticalScrollbar(false);
  }

  ClampScrollOffsetAfterOverflowChange();

  if (!scrollbars_are_frozen) {
    UpdateScrollableAreaSet();
  }

  DisableCompositingQueryAsserts disabler;
  PositionOverflowControls();
}
