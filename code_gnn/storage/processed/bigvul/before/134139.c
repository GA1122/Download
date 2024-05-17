void InputMethodIMM32::UpdateIMEState() {
  const HWND window_handle = GetAttachedWindowHandle(GetTextInputClient());
  const TextInputType text_input_type = GetTextInputType();
  const TextInputMode text_input_mode = GetTextInputMode();
  switch (text_input_type) {
    case ui::TEXT_INPUT_TYPE_NONE:
    case ui::TEXT_INPUT_TYPE_PASSWORD:
      imm32_manager_.DisableIME(window_handle);
      enabled_ = false;
      break;
    default:
      imm32_manager_.EnableIME(window_handle);
      enabled_ = true;
      break;
  }

  imm32_manager_.SetTextInputMode(window_handle, text_input_mode);
  tsf_inputscope::SetInputScopeForTsfUnawareWindow(
      window_handle, text_input_type, text_input_mode);
}
