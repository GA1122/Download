void RenderWidgetHostViewAura::OnWindowDestroying() {
#if defined(OS_WIN)
  HWND parent = NULL;
  if (!window_->GetRootWindow() || host_->is_hidden()) {
    parent = ui::GetHiddenWindow();
  } else {
    parent = window_->GetDispatcher()->host()->GetAcceleratedWidget();
  }
  LPARAM lparam = reinterpret_cast<LPARAM>(this);
  EnumChildWindows(parent, WindowDestroyingCallback, lparam);
#endif

  ui::InputMethod* input_method = GetInputMethod();
  if (input_method)
    input_method->DetachTextInputClient(this);
}
