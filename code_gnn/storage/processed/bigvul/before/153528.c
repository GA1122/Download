gfx::RectF GM2TabStyle::ScaleAndAlignBounds(const gfx::Rect& bounds,
                                            float scale,
                                            int stroke_thickness) {
  gfx::RectF aligned_bounds(bounds);
  const int corner_radius = GetCornerRadius();
  gfx::InsetsF layout_insets(stroke_thickness, corner_radius, stroke_thickness,
                             corner_radius + GetSeparatorSize().width());
  aligned_bounds.Inset(layout_insets);

  aligned_bounds.Scale(scale);

  const float x = std::round(aligned_bounds.x());
  const float y = std::round(aligned_bounds.y());
  const float right = std::round(aligned_bounds.right());
  const float bottom = std::round(aligned_bounds.bottom());
  aligned_bounds = gfx::RectF(x, y, right - x, bottom - y);

  aligned_bounds.Inset(-layout_insets.Scale(scale));
  return aligned_bounds;
}
