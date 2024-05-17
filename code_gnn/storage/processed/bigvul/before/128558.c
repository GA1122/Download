void Instance::EnableAutoscroll(const pp::Point& origin) {
  if (is_autoscroll_)
    return;

  pp::Size client_size = plugin_size_;
  if (v_scrollbar_.get())
    client_size.Enlarge(-GetScrollbarThickness(), 0);
  if (h_scrollbar_.get())
    client_size.Enlarge(0, -GetScrollbarThickness());

  if (autoscroll_anchor_.size().width() > client_size.width() ||
      autoscroll_anchor_.size().height() > client_size.height())
    return;

  autoscroll_rect_ = pp::Rect(
      pp::Point(origin.x() - autoscroll_anchor_.size().width() / 2,
                origin.y() - autoscroll_anchor_.size().height() / 2),
      autoscroll_anchor_.size());

  if (autoscroll_rect_.right() > client_size.width()) {
    autoscroll_rect_.set_x(
        client_size.width() - autoscroll_anchor_.size().width());
  }
  if (autoscroll_rect_.bottom() > client_size.height()) {
    autoscroll_rect_.set_y(
        client_size.height() - autoscroll_anchor_.size().height());
  }

  if (autoscroll_rect_.x() < 0)
    autoscroll_rect_.set_x(0);
  if (autoscroll_rect_.y() < 0)
    autoscroll_rect_.set_y(0);

  is_autoscroll_ = true;
  Invalidate(kAutoScrollId, autoscroll_rect_);

  ScheduleTimer(kAutoScrollId, kAutoScrollTimeoutMs);
}
