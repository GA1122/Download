void InputMethodLinuxX11::OnCommit(const base::string16& text) {
  TextInputClient* text_input_client = GetTextInputClient();
  if (text_input_client)
    text_input_client->InsertText(text);
}
