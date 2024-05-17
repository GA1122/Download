void TouchEventConverterEvdev::ProcessAbs(const input_event& input) {
  switch (input.code) {
    case ABS_MT_TOUCH_MAJOR:
      events_[current_slot_].radius_x = input.value / 2.0f;
      break;
    case ABS_MT_TOUCH_MINOR:
      events_[current_slot_].radius_y = input.value / 2.0f;
      break;
    case ABS_MT_POSITION_X:
      events_[current_slot_].x = input.value;
      break;
    case ABS_MT_POSITION_Y:
      events_[current_slot_].y = input.value;
      break;
    case ABS_MT_TRACKING_ID:
      UpdateTrackingId(current_slot_, input.value);
      break;
    case ABS_MT_PRESSURE:
      events_[current_slot_].pressure = ScalePressure(input.value);
      break;
    case ABS_MT_SLOT:
      if (input.value >= 0 &&
          static_cast<size_t>(input.value) < events_.size()) {
        current_slot_ = input.value;
      } else {
        LOG(ERROR) << "invalid touch event index: " << input.value;
        return;
      }
      break;
    default:
      DVLOG(5) << "unhandled code for EV_ABS: " << input.code;
      return;
  }
  events_[current_slot_].altered = true;
}
