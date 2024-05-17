bool GesturePoint::HasEnoughDataToEstablishRail() const {
  int dx = x_delta();
  int dy = y_delta();

  int delta_squared = dx * dx + dy * dy;
  return delta_squared > kMinScrollDeltaSquared;
}
