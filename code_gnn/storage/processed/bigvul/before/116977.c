bool GesturePoint::BreaksHorizontalRail() {
  float vx = XVelocity();
  float vy = YVelocity();
  return fabs(vy) > kRailBreakProportion * fabs(vx) + kMinRailBreakVelocity;
}
