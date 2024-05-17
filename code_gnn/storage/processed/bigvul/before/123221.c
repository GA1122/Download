void RenderWidgetHostViewAura::OnFocus(aura::Window* old_focused_window) {
  if (host_->ignore_input_events())
    return;

  host_->GotFocus();
  host_->SetActive(true);

  ui::InputMethod* input_method = GetInputMethod();
  if (input_method) {
    input_method->SetFocusedTextInputClient(this);
    host_->SetInputMethodActive(input_method->IsActive());
  } else {
    host_->SetInputMethodActive(false);
  }
}
