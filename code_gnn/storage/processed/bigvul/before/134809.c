int TouchEventConverterEvdev::NextTrackingId() {
  return next_tracking_id_++ & kMaxTrackingId;
}
