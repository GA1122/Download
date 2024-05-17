bool RenderViewImpl::WillHandleMouseEvent(const WebKit::WebMouseEvent& event) {
  possible_drag_event_info_.event_source =
      ui::DragDropTypes::DRAG_EVENT_SOURCE_MOUSE;
  possible_drag_event_info_.event_location =
      gfx::Point(event.globalX, event.globalY);
  pepper_helper_->WillHandleMouseEvent();

  return mouse_lock_dispatcher_->WillHandleMouseEvent(event);
}
