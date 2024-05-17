input_ime::AutoCapitalizeType ImeObserver::ConvertInputContextAutoCapitalize(
    ui::IMEEngineHandlerInterface::InputContext input_context) {
  if (input_context.flags & ui::TEXT_INPUT_FLAG_AUTOCAPITALIZE_NONE)
    return input_ime::AUTO_CAPITALIZE_TYPE_NONE;
  if (input_context.flags & ui::TEXT_INPUT_FLAG_AUTOCAPITALIZE_CHARACTERS)
    return input_ime::AUTO_CAPITALIZE_TYPE_CHARACTERS;
  if (input_context.flags & ui::TEXT_INPUT_FLAG_AUTOCAPITALIZE_WORDS)
    return input_ime::AUTO_CAPITALIZE_TYPE_WORDS;
  return input_ime::AUTO_CAPITALIZE_TYPE_SENTENCES;
}
