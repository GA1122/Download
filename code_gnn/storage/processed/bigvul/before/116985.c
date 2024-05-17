bool GesturePoint::IsInHorizontalRailWindow() const {
  int dx = x_delta();
  int dy = y_delta();
  return abs(dx) > kRailStartProportion * abs(dy);
}
