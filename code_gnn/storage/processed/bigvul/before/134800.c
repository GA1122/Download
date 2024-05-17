void TabletEventConverterEvdev::UpdateCursor() {
  gfx::Rect confined_bounds = cursor_->GetCursorConfinedBounds();

  int x =
      ((x_abs_location_ - x_abs_min_) * confined_bounds.width()) / x_abs_range_;
  int y = ((y_abs_location_ - y_abs_min_) * confined_bounds.height()) /
          y_abs_range_;

  x += confined_bounds.x();
  y += confined_bounds.y();

  cursor_->MoveCursorTo(gfx::PointF(x, y));
}
