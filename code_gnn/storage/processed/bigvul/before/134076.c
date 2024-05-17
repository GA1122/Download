void InputMethodBase::DetachTextInputClient(TextInputClient* client) {
  if (text_input_client_ == client) {
    OnWillChangeFocusedClient(client, NULL);
    text_input_client_ = NULL;
    is_sticky_text_input_client_ = false;
    OnDidChangeFocusedClient(client, NULL);
    NotifyTextInputStateChanged(text_input_client_);
  }
}
