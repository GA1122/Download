void ScrollableShelfView::UpdateGradientZone() {
  gfx::Rect zone_rect;
  bool fade_in;
  const int zone_length = GetFadeZoneLength();
  const bool is_horizontal_alignment = GetShelf()->IsHorizontalAlignment();

  if (right_arrow_->GetVisible()) {
    const gfx::Rect right_arrow_bounds = right_arrow_->bounds();
    zone_rect = is_horizontal_alignment
                    ? gfx::Rect(right_arrow_bounds.x() - zone_length, 0,
                                zone_length, height())
                    : gfx::Rect(0, right_arrow_bounds.y() - zone_length,
                                width(), zone_length);
    fade_in = false;
  } else if (left_arrow_->GetVisible()) {
    const gfx::Rect left_arrow_bounds = left_arrow_->bounds();
    zone_rect =
        is_horizontal_alignment
            ? gfx::Rect(left_arrow_bounds.right(), 0, zone_length, height())
            : gfx::Rect(0, left_arrow_bounds.bottom(), width(), zone_length);
    fade_in = true;
  } else {
    zone_rect = gfx::Rect();
    fade_in = false;
  }

  GradientLayerDelegate::FadeZone fade_zone = {zone_rect, fade_in,
                                               is_horizontal_alignment};
  gradient_layer_delegate_->set_fade_zone(fade_zone);
  SchedulePaint();
}
