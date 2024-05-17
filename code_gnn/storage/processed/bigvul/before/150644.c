cc::SnapFlingController::GestureScrollUpdateInfo GetGestureScrollUpdateInfo(
    const WebGestureEvent& event) {
  cc::SnapFlingController::GestureScrollUpdateInfo info;
  info.delta = gfx::Vector2dF(-event.data.scroll_update.delta_x,
                              -event.data.scroll_update.delta_y);
  info.is_in_inertial_phase =
      event.data.scroll_update.inertial_phase ==
      blink::WebGestureEvent::InertialPhaseState::kMomentum;
  info.event_time = event.TimeStamp();
  return info;
}
