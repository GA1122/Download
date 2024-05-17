cc::ScrollState CreateScrollStateForGesture(const WebGestureEvent& event) {
  cc::ScrollStateData scroll_state_data;
  switch (event.GetType()) {
    case WebInputEvent::kGestureScrollBegin:
      scroll_state_data.position_x = event.PositionInWidget().x;
      scroll_state_data.position_y = event.PositionInWidget().y;
      scroll_state_data.delta_x_hint = -event.data.scroll_begin.delta_x_hint;
      scroll_state_data.delta_y_hint = -event.data.scroll_begin.delta_y_hint;
      scroll_state_data.is_beginning = true;
      scroll_state_data.is_in_inertial_phase =
          (event.data.scroll_begin.inertial_phase ==
           WebGestureEvent::InertialPhaseState::kMomentum);
      scroll_state_data.delta_granularity =
          static_cast<double>(event.data.scroll_begin.delta_hint_units);
      break;
    case WebInputEvent::kGestureScrollUpdate:
      scroll_state_data.delta_x = -event.data.scroll_update.delta_x;
      scroll_state_data.delta_y = -event.data.scroll_update.delta_y;
      scroll_state_data.velocity_x = event.data.scroll_update.velocity_x;
      scroll_state_data.velocity_y = event.data.scroll_update.velocity_y;
      scroll_state_data.is_in_inertial_phase =
          event.data.scroll_update.inertial_phase ==
          WebGestureEvent::InertialPhaseState::kMomentum;
      scroll_state_data.delta_granularity =
          static_cast<double>(event.data.scroll_update.delta_units);
      break;
    case WebInputEvent::kGestureScrollEnd:
      scroll_state_data.is_ending = true;
      break;
    default:
      NOTREACHED();
      break;
  }
  return cc::ScrollState(scroll_state_data);
}
