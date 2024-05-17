bool RenderViewImpl::WillHandleGestureEvent(
    const WebKit::WebGestureEvent& event) {
  possible_drag_event_info_.event_source =
      ui::DragDropTypes::DRAG_EVENT_SOURCE_TOUCH;
  possible_drag_event_info_.event_location =
      gfx::Point(event.globalX, event.globalY);
  return false;
}
