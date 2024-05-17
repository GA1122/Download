bool RenderWidgetHostViewAura::ShouldActivate() const {
  const ui::Event* event = window_->GetRootWindow()->current_event();
  if (!event)
    return true;
  return is_fullscreen_;
}
