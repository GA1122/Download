void RenderWidgetHostViewAura::AddedToRootWindow() {
  window_->GetDispatcher()->AddRootWindowObserver(this);
  host_->ParentChanged(GetNativeViewId());
  UpdateScreenInfo(window_);
  if (popup_type_ != blink::WebPopupTypeNone)
    event_filter_for_popup_exit_.reset(new EventFilterForPopupExit(this));

  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(window_->GetRootWindow());
  if (cursor_client) {
    cursor_client->AddObserver(this);
    NotifyRendererOfCursorVisibilityState(cursor_client->IsCursorVisible());
  }
  if (current_surface_.get())
    UpdateExternalTexture();
  if (HasFocus()) {
    ui::InputMethod* input_method = GetInputMethod();
    if (input_method)
      input_method->SetFocusedTextInputClient(this);
  }
}
