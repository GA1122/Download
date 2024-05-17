void TabletEventConverterEvdev::FlushEvents(const input_event& input) {
  if (!cursor_)
    return;

  if (stylus_ == 0) {
    abs_value_dirty_ = false;
    return;
  }

  if (!abs_value_dirty_)
    return;

  UpdateCursor();

  dispatcher_->DispatchMouseMoveEvent(
      MouseMoveEventParams(input_device_.id, cursor_->GetLocation(),
                           TimeDeltaFromInputEvent(input)));

  abs_value_dirty_ = false;
}
