FillLightMode ParseFillLightMode(const String& blink_mode) {
  if (blink_mode == "off")
    return FillLightMode::OFF;
  if (blink_mode == "auto")
    return FillLightMode::AUTO;
  if (blink_mode == "flash")
    return FillLightMode::FLASH;
  NOTREACHED();
  return FillLightMode::OFF;
}
