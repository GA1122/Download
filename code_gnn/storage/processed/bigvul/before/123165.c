gfx::Rect RenderWidgetHostViewAura::GetBoundsInRootWindow() {
  return window_->GetToplevelWindow()->GetBoundsInScreen();
}
