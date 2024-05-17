void TouchEventConverterEvdev::ProcessMultitouchEvent(
    const input_event& input) {
  if (input.type == EV_SYN) {
    ProcessSyn(input);
  } else if (syn_dropped_) {
  } else if (input.type == EV_ABS) {
    if (events_.size() <= current_slot_) {
      LOG(ERROR) << "current_slot_ (" << current_slot_
                 << ") >= events_.size() (" << events_.size() << ")";
    } else {
      ProcessAbs(input);
    }
  } else if (input.type == EV_KEY) {
    ProcessKey(input);
  } else {
    NOTIMPLEMENTED() << "invalid type: " << input.type;
  }
}
