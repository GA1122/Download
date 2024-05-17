static LayoutRect InvalidatePaintOfScrollbarIfNeeded(
    Scrollbar* scrollbar,
    GraphicsLayer* graphics_layer,
    bool& previously_was_overlay,
    const LayoutRect& previous_visual_rect,
    bool needs_paint_invalidation_arg,
    LayoutBox& box,
    const PaintInvalidatorContext& context) {
  bool is_overlay = scrollbar && scrollbar->IsOverlayScrollbar();

  LayoutRect new_visual_rect;
  if (scrollbar && !(graphics_layer && is_overlay)) {
    new_visual_rect = ScrollControlVisualRect(scrollbar->FrameRect(), box,
                                              context, previous_visual_rect);
  }

  bool needs_paint_invalidation = needs_paint_invalidation_arg;
  if (needs_paint_invalidation && graphics_layer) {
    needs_paint_invalidation = false;
    DCHECK(!graphics_layer->PaintsContentOrHitTest() ||
           graphics_layer->GetPaintController().GetPaintArtifact().IsEmpty());
  }

  const LayoutBoxModelObject& paint_invalidation_container =
      *context.paint_invalidation_container;
  LayoutSize new_scrollbar_used_space_in_box;
  if (!is_overlay)
    new_scrollbar_used_space_in_box = new_visual_rect.Size();
  LayoutSize previous_scrollbar_used_space_in_box;
  if (!previously_was_overlay)
    previous_scrollbar_used_space_in_box = previous_visual_rect.Size();

  if (!(new_scrollbar_used_space_in_box.IsEmpty() &&
        previous_scrollbar_used_space_in_box.IsEmpty()) &&
      new_scrollbar_used_space_in_box != previous_scrollbar_used_space_in_box) {
    context.painting_layer->SetNeedsRepaint();
    ObjectPaintInvalidator(box).InvalidateDisplayItemClient(
        box, PaintInvalidationReason::kGeometry);
  }

  bool invalidated = InvalidatePaintOfScrollControlIfNeeded(
      new_visual_rect, previous_visual_rect, needs_paint_invalidation, box,
      paint_invalidation_container);

  previously_was_overlay = is_overlay;

  if (!invalidated || !scrollbar || graphics_layer)
    return new_visual_rect;

  context.painting_layer->SetNeedsRepaint();
  ObjectPaintInvalidator(box).InvalidateDisplayItemClient(
      *scrollbar, PaintInvalidationReason::kScrollControl);
  if (scrollbar->IsCustomScrollbar()) {
    ToLayoutScrollbar(scrollbar)
        ->InvalidateDisplayItemClientsOfScrollbarParts();
  }

  return new_visual_rect;
}
