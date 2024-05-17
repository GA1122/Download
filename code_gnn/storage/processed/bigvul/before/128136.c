void CastCastView::Layout() {
  views::View::Layout();

  gfx::Size stop_size = stop_button_->GetPreferredSize();
  gfx::Rect stop_bounds(stop_size);
  stop_bounds.set_x(width() - stop_size.width() - kStopButtonRightPadding);
  stop_bounds.set_y((height() - stop_size.height()) / 2);
  stop_button_->SetBoundsRect(stop_bounds);

  if (label_container_->bounds().Intersects(stop_button_->bounds())) {
    gfx::Rect label_bounds = label_container_->bounds();
    label_bounds.set_width(stop_button_->x() - kTrayPopupPaddingBetweenItems -
                           label_container_->x());
    label_container_->SetBoundsRect(label_bounds);
  }

  const int extra_height =
      height() - label_container_->GetPreferredSize().height();
  label_container_->SetY(extra_height / 2);
}
