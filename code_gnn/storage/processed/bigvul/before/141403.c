void PaintLayerScrollableArea::DidChangeGlobalRootScroller() {
  if (GetLayoutBox()->GetNode()->IsElementNode()) {
    ToElement(GetLayoutBox()->GetNode())->SetNeedsCompositingUpdate();
    GetLayoutBox()->SetNeedsPaintPropertyUpdate();
  }

  if (GetLayoutBox()->GetFrame()->GetSettings() &&
      GetLayoutBox()->GetFrame()->GetSettings()->GetViewportEnabled()) {
    bool needs_horizontal_scrollbar;
    bool needs_vertical_scrollbar;
    ComputeScrollbarExistence(needs_horizontal_scrollbar,
                              needs_vertical_scrollbar);
    SetHasHorizontalScrollbar(needs_horizontal_scrollbar);
    SetHasVerticalScrollbar(needs_vertical_scrollbar);
  }
}
