void PaintController::GenerateIncrementalRasterInvalidation(
    PaintChunk& chunk,
    const DisplayItem& old_item,
    const DisplayItem& new_item) {
  DCHECK(&old_item.Client() == &new_item.Client());
  FloatRect old_visual_rect(old_item.VisualRect());
  FloatRect new_visual_rect(new_item.VisualRect());
  DCHECK(old_visual_rect.Location() == new_visual_rect.Location());

  FloatRect right_delta =
      ComputeRightDelta(new_visual_rect.Location(), old_visual_rect.Size(),
                        new_visual_rect.Size());
  if (!right_delta.IsEmpty()) {
    AddRasterInvalidation(new_item.Client(), chunk, right_delta,
                          PaintInvalidationReason::kIncremental);
  }

  FloatRect bottom_delta =
      ComputeBottomDelta(new_visual_rect.Location(), old_visual_rect.Size(),
                         new_visual_rect.Size());
  if (!bottom_delta.IsEmpty()) {
    AddRasterInvalidation(new_item.Client(), chunk, bottom_delta,
                          PaintInvalidationReason::kIncremental);
  }
}
