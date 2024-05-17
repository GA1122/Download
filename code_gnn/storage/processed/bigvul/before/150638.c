cc::ScrollState CreateScrollStateForInertialEnd() {
  cc::ScrollStateData scroll_state_data;
  scroll_state_data.is_ending = true;
  return cc::ScrollState(scroll_state_data);
}
