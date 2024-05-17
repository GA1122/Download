void EventConverterEvdevImpl::FlushEvents(const input_event& input) {
  if (!cursor_ || (x_offset_ == 0 && y_offset_ == 0))
    return;

  cursor_->MoveCursor(gfx::Vector2dF(x_offset_, y_offset_));

  dispatcher_->DispatchMouseMoveEvent(
      MouseMoveEventParams(input_device_.id, cursor_->GetLocation(),
                           TimeDeltaFromInputEvent(input)));

  x_offset_ = 0;
  y_offset_ = 0;
}
