void TouchEventConverterEvdev::ReleaseTouches() {
  for (size_t slot = 0; slot < events_.size(); slot++)
    UpdateTrackingId(slot, kTrackingIdForUnusedSlot);

  ReportEvents(EventTimeForNow());
}
