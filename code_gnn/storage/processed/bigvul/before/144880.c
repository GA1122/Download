void RenderWidgetHostViewAura::ApplyEventFilterForPopupExit(
    ui::LocatedEvent* event) {
  if (in_shutdown_ || is_fullscreen_ || !event->target())
    return;

  if (event->type() != ui::ET_MOUSE_PRESSED &&
      event->type() != ui::ET_TOUCH_PRESSED) {
    return;
  }

  aura::Window* target = static_cast<aura::Window*>(event->target());
  if (target != window_ &&
      (!popup_parent_host_view_ ||
       target != popup_parent_host_view_->window_)) {
    if (popup_parent_host_view_ && popup_parent_host_view_->host_) {
      popup_parent_host_view_->event_handler()
          ->set_focus_on_mouse_down_or_key_event(true);
      popup_parent_host_view_->host_->Blur();
    }
    Shutdown();
  }
}
