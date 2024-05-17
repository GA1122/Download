void PaintController::GenerateFullRasterInvalidation(
    PaintChunk& chunk,
    const DisplayItem& old_item,
    const DisplayItem& new_item) {
  DCHECK(&old_item.Client() == &new_item.Client());
  FloatRect old_visual_rect(old_item.VisualRect());
  FloatRect new_visual_rect(new_item.VisualRect());

  if (!new_visual_rect.Contains(old_visual_rect)) {
    AddRasterInvalidation(new_item.Client(), chunk, old_visual_rect,
                          new_item.Client().GetPaintInvalidationReason());
    if (old_visual_rect.Contains(new_visual_rect))
      return;
  }

  AddRasterInvalidation(new_item.Client(), chunk, new_visual_rect,
                        new_item.Client().GetPaintInvalidationReason());
}
