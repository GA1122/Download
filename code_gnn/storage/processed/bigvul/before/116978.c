bool GesturePoint::BreaksVerticalRail() {
  float vx = XVelocity();
  float vy = YVelocity();
  return fabs(vx) > kRailBreakProportion * fabs(vy) + kMinRailBreakVelocity;
}
