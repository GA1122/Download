void InputMethodBase::SetFocusedTextInputClientInternal(
    TextInputClient* client) {
  TextInputClient* old = text_input_client_;
  OnWillChangeFocusedClient(old, client);
  text_input_client_ = client;   
  OnDidChangeFocusedClient(old, client);
  NotifyTextInputStateChanged(text_input_client_);
}
