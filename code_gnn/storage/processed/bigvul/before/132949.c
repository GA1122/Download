ui::Compositor* RenderWidgetHostViewAura::GetCompositor() const {
  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  return dispatcher ? dispatcher->host()->compositor() : NULL;
}
