void InputMethodLinuxX11::OnPreeditEnd() {
  TextInputClient* text_input_client = GetTextInputClient();
  if (text_input_client && text_input_client->HasCompositionText())
    text_input_client->ClearCompositionText();
}
