void InputMethodIBus::SetContext(IBusInputContext* ic) {
  DCHECK(ic);
  DCHECK(!context_);
  context_ = ic;

  g_signal_connect(ic, "commit-text",
                   G_CALLBACK(OnCommitTextThunk), this);
  g_signal_connect(ic, "forward-key-event",
                   G_CALLBACK(OnForwardKeyEventThunk), this);
  g_signal_connect(ic, "update-preedit-text",
                   G_CALLBACK(OnUpdatePreeditTextThunk), this);
  g_signal_connect(ic, "show-preedit-text",
                   G_CALLBACK(OnShowPreeditTextThunk), this);
  g_signal_connect(ic, "hide-preedit-text",
                   G_CALLBACK(OnHidePreeditTextThunk), this);
  g_signal_connect(ic, "destroy",
                   G_CALLBACK(OnDestroyThunk), this);

  ibus_client_->SetCapabilities(ic, internal::IBusClient::INLINE_COMPOSITION);

  UpdateContextFocusState();
  OnCaretBoundsChanged(GetTextInputClient());
  OnInputMethodChanged();
}
