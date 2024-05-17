void TouchEventConverterEvdev::ProcessSyn(const input_event& input) {
  switch (input.code) {
    case SYN_REPORT:
      if (syn_dropped_) {
        if (Reinitialize()) {
          syn_dropped_ = false;
        } else {
          LOG(ERROR) << "failed to re-initialize device info";
        }
      } else {
        ReportEvents(EventConverterEvdev::TimeDeltaFromInputEvent(input));
      }
      break;
    case SYN_DROPPED:
      syn_dropped_ = true;
      break;
    default:
      NOTIMPLEMENTED() << "invalid code for EV_SYN: " << input.code;
  }
}
