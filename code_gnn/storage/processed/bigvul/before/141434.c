void PaintLayerScrollableArea::InvalidatePaintOfScrollControlsIfNeeded(
    const PaintInvalidatorContext& context) {
  LayoutBox& box = *GetLayoutBox();
  SetHorizontalScrollbarVisualRect(InvalidatePaintOfScrollbarIfNeeded(
      HorizontalScrollbar(), LayerForHorizontalScrollbar(),
      horizontal_scrollbar_previously_was_overlay_,
      horizontal_scrollbar_visual_rect_,
      HorizontalScrollbarNeedsPaintInvalidation(), box, context));
  SetVerticalScrollbarVisualRect(InvalidatePaintOfScrollbarIfNeeded(
      VerticalScrollbar(), LayerForVerticalScrollbar(),
      vertical_scrollbar_previously_was_overlay_,
      vertical_scrollbar_visual_rect_,
      VerticalScrollbarNeedsPaintInvalidation(), box, context));

  LayoutRect scroll_corner_and_resizer_visual_rect =
      ScrollControlVisualRect(ScrollCornerAndResizerRect(), box, context,
                              scroll_corner_and_resizer_visual_rect_);
  const LayoutBoxModelObject& paint_invalidation_container =
      *context.paint_invalidation_container;
  if (InvalidatePaintOfScrollControlIfNeeded(
          scroll_corner_and_resizer_visual_rect,
          scroll_corner_and_resizer_visual_rect_,
          ScrollCornerNeedsPaintInvalidation(), box,
          paint_invalidation_container)) {
    SetScrollCornerAndResizerVisualRect(scroll_corner_and_resizer_visual_rect);
    if (LayoutScrollbarPart* scroll_corner = ScrollCorner()) {
      ObjectPaintInvalidator(*scroll_corner)
          .InvalidateDisplayItemClientsIncludingNonCompositingDescendants(
              PaintInvalidationReason::kScrollControl);
    }
    if (LayoutScrollbarPart* resizer = Resizer()) {
      ObjectPaintInvalidator(*resizer)
          .InvalidateDisplayItemClientsIncludingNonCompositingDescendants(
              PaintInvalidationReason::kScrollControl);
    }
  }

  ClearNeedsPaintInvalidationForScrollControls();
}
