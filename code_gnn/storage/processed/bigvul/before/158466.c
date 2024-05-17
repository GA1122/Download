  void SimulateMouseMove(int x, int y, int modifiers) {
    SimulateMouseEvent(WebInputEvent::kMouseMove, x, y, modifiers, false);
  }
