bool PaintController::LastDisplayItemIsNoopBegin() const {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV175Enabled());

  if (new_display_item_list_.IsEmpty())
    return false;

  const auto& last_display_item = new_display_item_list_.Last();
  return last_display_item.IsBegin() && !last_display_item.DrawsContent();
}
