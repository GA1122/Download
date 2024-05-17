WebTextInputMode InputMethodController::InputModeOfFocusedElement() const {
  if (!RuntimeEnabledFeatures::InputModeAttributeEnabled())
    return kWebTextInputModeDefault;

  AtomicString mode = GetInputModeAttribute(GetDocument().FocusedElement());

  if (mode.IsEmpty())
    return kWebTextInputModeDefault;
  if (mode == InputModeNames::verbatim)
    return kWebTextInputModeVerbatim;
  if (mode == InputModeNames::latin)
    return kWebTextInputModeLatin;
  if (mode == InputModeNames::latin_name)
    return kWebTextInputModeLatinName;
  if (mode == InputModeNames::latin_prose)
    return kWebTextInputModeLatinProse;
  if (mode == InputModeNames::full_width_latin)
    return kWebTextInputModeFullWidthLatin;
  if (mode == InputModeNames::kana)
    return kWebTextInputModeKana;
  if (mode == InputModeNames::kana_name)
    return kWebTextInputModeKanaName;
  if (mode == InputModeNames::katakana)
    return kWebTextInputModeKataKana;
  if (mode == InputModeNames::numeric)
    return kWebTextInputModeNumeric;
  if (mode == InputModeNames::tel)
    return kWebTextInputModeTel;
  if (mode == InputModeNames::email)
    return kWebTextInputModeEmail;
  if (mode == InputModeNames::url)
    return kWebTextInputModeUrl;
  return kWebTextInputModeDefault;
}
