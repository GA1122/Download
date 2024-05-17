void InputImeAPI::Shutdown() {
  EventRouter::Get(browser_context_)->UnregisterObserver(this);
  registrar_.RemoveAll();
  if (observer_ && ui::IMEBridge::Get()) {
    ui::IMEBridge::Get()->SetObserver(nullptr);
  }
}
