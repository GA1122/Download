InputMethodIBus::~InputMethodIBus() {
  AbandonAllPendingKeyEvents();
  context_focused_ = false;
  ConfirmCompositionText();
  OnInputMethodChanged();

  chromeos::IBusBridge::Get()->SetInputContextHandler(NULL);
}
