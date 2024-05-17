void InputMethodTSF::ConfirmCompositionText() {
  if (!IsTextInputTypeNone())
    ui::TSFBridge::GetInstance()->ConfirmComposition();
}
