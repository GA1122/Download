TabStyle::SeparatorBounds GM2TabStyle::GetSeparatorBounds(float scale) const {
  const gfx::RectF aligned_bounds =
      ScaleAndAlignBounds(tab_->bounds(), scale, GetStrokeThickness());
  const int corner_radius = GetCornerRadius() * scale;
  gfx::SizeF separator_size(GetSeparatorSize());
  separator_size.Scale(scale);

  SeparatorBounds separator_bounds;

  separator_bounds.leading =
      gfx::RectF(aligned_bounds.x() + corner_radius,
                 aligned_bounds.y() +
                     (aligned_bounds.height() - separator_size.height()) / 2,
                 separator_size.width(), separator_size.height());

  separator_bounds.trailing = separator_bounds.leading;
  separator_bounds.trailing.set_x(aligned_bounds.right() -
                                  (corner_radius + separator_size.width()));

  gfx::PointF origin(tab_->bounds().origin());
  origin.Scale(scale);
  separator_bounds.leading.Offset(-origin.x(), -origin.y());
  separator_bounds.trailing.Offset(-origin.x(), -origin.y());

  return separator_bounds;
}
