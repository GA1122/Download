void UiSceneCreator::CreateKeyboard() {
  auto scaler = base::MakeUnique<ScaledDepthAdjuster>(kKeyboardDistance);
  scaler->SetName(kKeyboardDmmRoot);
  scene_->AddUiElement(kRoot, std::move(scaler));

  auto keyboard = base::MakeUnique<Keyboard>();
  keyboard->SetKeyboardDelegate(keyboard_delegate_);
  keyboard->SetDrawPhase(kPhaseForeground);
  keyboard->SetTranslate(0.0, kKeyboardVerticalOffsetDMM, 0.0);
  keyboard->AddBinding(VR_BIND_FUNC(bool, Model, model_, editing_input,
                                    UiElement, keyboard.get(), SetVisible));
  scene_->AddUiElement(kKeyboardDmmRoot, std::move(keyboard));
}
