bool GestureProviderAura::IsConsideredDoubleTap(
    const GestureEventData& previous_tap,
    const GestureEventData& current_tap) const {
  if (current_tap.time - previous_tap.time >
      base::TimeDelta::FromMilliseconds(
          ui::GestureConfiguration::max_seconds_between_double_click() *
          1000)) {
    return false;
  }

  double double_tap_slop_square =
      GestureConfiguration::max_distance_between_taps_for_double_tap();
  double_tap_slop_square *= double_tap_slop_square;
  const float delta_x = previous_tap.x - current_tap.x;
  const float delta_y = previous_tap.y - current_tap.y;
  return (delta_x * delta_x + delta_y * delta_y < double_tap_slop_square);
}
