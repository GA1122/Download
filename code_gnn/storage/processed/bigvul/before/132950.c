ui::InputMethod* RenderWidgetHostViewAura::GetInputMethod() const {
  aura::Window* root_window = window_->GetRootWindow();
  if (!root_window)
    return NULL;
  return root_window->GetProperty(aura::client::kRootWindowInputMethodKey);
}
