void RenderWidgetHostViewAura::OnMouseEvent(ui::MouseEvent* event) {
#if defined(OS_WIN)
  if (event->type() == ui::ET_MOUSE_MOVED) {
    if (event->location() == last_mouse_move_location_) {
      event->SetHandled();
      return;
    }
    last_mouse_move_location_ = event->location();
  }
#endif
  event_handler_->OnMouseEvent(event);
}
