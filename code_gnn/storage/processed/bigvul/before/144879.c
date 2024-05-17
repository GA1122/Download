void RenderWidgetHostViewAura::AddedToRootWindow() {
  window_->GetHost()->AddObserver(this);
  UpdateScreenInfo(window_);

  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(window_->GetRootWindow());
  if (cursor_client) {
    cursor_client->AddObserver(this);
    NotifyRendererOfCursorVisibilityState(cursor_client->IsCursorVisible());
  }
  if (HasFocus()) {
    ui::InputMethod* input_method = GetInputMethod();
    if (input_method)
      input_method->SetFocusedTextInputClient(this);
  }

#if defined(OS_WIN)
  UpdateLegacyWin();
#endif

  if (delegated_frame_host_)
    delegated_frame_host_->SetCompositor(window_->GetHost()->compositor());
}
