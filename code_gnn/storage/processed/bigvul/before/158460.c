  void SimulateGesturePinchUpdateEvent(float scale,
                                       float anchorX,
                                       float anchorY,
                                       int modifiers) {
    SimulateGestureEventCore(SyntheticWebGestureEventBuilder::BuildPinchUpdate(
        scale, anchorX, anchorY, modifiers,
        blink::kWebGestureDeviceTouchscreen));
  }
