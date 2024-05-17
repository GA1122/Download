EventType TouchEventConverterEvdev::GetEventTypeForTouch(
    const InProgressTouchEvdev& touch) {
  if (touch.cancelled)
    return ET_UNKNOWN;

  if (touch_noise_finder_ && touch_noise_finder_->SlotHasNoise(touch.slot)) {
    if (touch.touching && !touch.was_touching)
      return ET_UNKNOWN;
    return ET_TOUCH_CANCELLED;
  }

  if (touch.touching)
    return touch.was_touching ? ET_TOUCH_MOVED : ET_TOUCH_PRESSED;
  return touch.was_touching ? ET_TOUCH_RELEASED : ET_UNKNOWN;
}
