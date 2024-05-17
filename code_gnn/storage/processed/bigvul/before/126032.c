void AutomationMouseEventProcessor::OnWillProcessMouseEventAt(
    const gfx::Point& point) {
  has_point_ = true;
  point_ = point;
}
