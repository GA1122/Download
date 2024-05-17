void TouchEventConverterEvdev::ReportEvent(const InProgressTouchEvdev& event,
                                           EventType event_type,
                                           const base::TimeDelta& timestamp) {
  dispatcher_->DispatchTouchEvent(TouchEventParams(
      input_device_.id, event.slot, event_type, gfx::PointF(event.x, event.y),
      gfx::Vector2dF(event.radius_x, event.radius_y), event.pressure,
      timestamp));
}
