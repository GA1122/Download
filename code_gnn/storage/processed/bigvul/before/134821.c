void TouchEventConverterEvdev::UpdateTrackingId(int slot, int tracking_id) {
  InProgressTouchEvdev* event = &events_[slot];

  if (event->tracking_id == tracking_id)
    return;

  event->tracking_id = tracking_id;
  event->touching = (tracking_id >= 0);
  event->altered = true;

  if (tracking_id >= 0)
    event->cancelled = false;
}
