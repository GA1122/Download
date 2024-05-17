void RenderWidgetHostViewAura::Focus() {
  aura::client::FocusClient* client = aura::client::GetFocusClient(window_);
  if (client)
    window_->Focus();
}
