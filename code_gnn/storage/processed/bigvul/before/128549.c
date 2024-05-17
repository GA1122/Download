void Instance::DisableAutoscroll() {
  if (is_autoscroll_) {
    is_autoscroll_ = false;
    Invalidate(kAutoScrollId, autoscroll_rect_);
  }
}
