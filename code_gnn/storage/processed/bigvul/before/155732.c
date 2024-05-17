bool GamepadBuilder::IsValid() const {
  switch (mapping_) {
    case GamepadMapping::kXRStandard:
      return gamepad_.axes_length >= 2 && gamepad_.buttons_length >= 2;
    case GamepadMapping::kStandard:
    case GamepadMapping::kNone:
      return true;
  }

  NOTREACHED();
}
