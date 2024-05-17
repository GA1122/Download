void InputMethodTSF::DetachTextInputClient(TextInputClient* client) {
  InputMethodWin::DetachTextInputClient(client);
  ui::TSFBridge::GetInstance()->RemoveFocusedClient(client);
}
