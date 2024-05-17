void RenderWidgetHostViewAura::OnWindowDestroyed(aura::Window* window) {
  delete this;
}
