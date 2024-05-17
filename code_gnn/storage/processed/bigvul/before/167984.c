void LocalFrame::SetViewportIntersectionFromParent(
    const IntRect& viewport_intersection) {
  if (remote_viewport_intersection_ != viewport_intersection) {
    remote_viewport_intersection_ = viewport_intersection;
    if (View())
      View()->ScheduleAnimation();
  }
}
