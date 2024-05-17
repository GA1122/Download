static bool InvalidatePaintOfScrollControlIfNeeded(
    const LayoutRect& new_visual_rect,
    const LayoutRect& previous_visual_rect,
    bool needs_paint_invalidation,
    LayoutBox& box,
    const LayoutBoxModelObject& paint_invalidation_container) {
  bool should_invalidate_new_rect = needs_paint_invalidation;
  if (new_visual_rect != previous_visual_rect) {
    should_invalidate_new_rect = true;
  } else if (previous_visual_rect.IsEmpty()) {
    DCHECK(new_visual_rect.IsEmpty());
    should_invalidate_new_rect = false;
  }

  return should_invalidate_new_rect;
}
