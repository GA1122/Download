void HTMLMediaElement::ActivateViewportIntersectionMonitoring(bool activate) {
  if (activate && !check_viewport_intersection_timer_.IsActive()) {
    check_viewport_intersection_timer_.StartRepeating(
        kCheckViewportIntersectionInterval, FROM_HERE);
  } else if (!activate) {
    check_viewport_intersection_timer_.Stop();
  }
}
