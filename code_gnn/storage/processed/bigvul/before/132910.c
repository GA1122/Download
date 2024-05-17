void RenderWidgetHostViewAura::ApplyEventFilterForPopupExit(
    ui::MouseEvent* event) {
  if (in_shutdown_ || is_fullscreen_)
    return;

  if (event->type() != ui::ET_MOUSE_PRESSED || !event->target())
    return;

  aura::Window* target = static_cast<aura::Window*>(event->target());
  if (target != window_ &&
      (!popup_parent_host_view_ ||
       target != popup_parent_host_view_->window_)) {
    in_shutdown_ = true;
    host_->Shutdown();
  }
}
