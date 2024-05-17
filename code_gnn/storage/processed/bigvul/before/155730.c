std::string GamepadMappingToString(GamepadBuilder::GamepadMapping mapping) {
  switch (mapping) {
    case GamepadBuilder::GamepadMapping::kNone:
      return "";
      break;
    case GamepadBuilder::GamepadMapping::kStandard:
      return "standard";
      break;
    case GamepadBuilder::GamepadMapping::kXRStandard:
      return "xr-standard";
      break;
  }

  NOTREACHED();
}
