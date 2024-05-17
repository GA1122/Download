bool InsertText(const base::string16& text) {
  keyboard::KeyboardController* controller = KeyboardController::GetInstance();
  if (!controller)
    return false;

  ui::InputMethod* input_method = controller->proxy()->GetInputMethod();
  if (!input_method)
    return false;

  ui::TextInputClient* tic = input_method->GetTextInputClient();
  if (!tic || tic->GetTextInputType() == ui::TEXT_INPUT_TYPE_NONE)
    return false;

  tic->InsertText(text);

  return true;
}
