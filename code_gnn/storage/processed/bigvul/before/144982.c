void RenderWidgetHostViewAura::SetTooltipsEnabled(bool enable) {
  if (enable) {
    tooltip_disabler_.reset();
  } else {
    tooltip_disabler_.reset(
        new aura::client::ScopedTooltipDisabler(window_->GetRootWindow()));
  }
}
