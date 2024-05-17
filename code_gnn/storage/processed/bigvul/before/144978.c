void RenderWidgetHostViewAura::SetOverscrollControllerEnabled(bool enabled) {
  if (!enabled)
    overscroll_controller_.reset();
  else if (!overscroll_controller_)
    overscroll_controller_.reset(new OverscrollController());
}
