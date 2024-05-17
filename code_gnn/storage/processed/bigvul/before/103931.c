void KeyboardOverlayHandler::GetInputMethodId(const ListValue* args) {
  chromeos::input_method::InputMethodManager* manager =
      chromeos::input_method::InputMethodManager::GetInstance();
  const chromeos::input_method::InputMethodDescriptor& descriptor =
      manager->current_input_method();
  StringValue param(descriptor.id());
  web_ui_->CallJavascriptFunction("initKeyboardOverlayId", param);
}
