bool GesturePoint::IsInVerticalRailWindow() const {
  int dx = x_delta();
  int dy = y_delta();
  return abs(dy) > kRailStartProportion * abs(dx);
}
