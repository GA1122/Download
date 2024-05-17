blink::WebScreenOrientationType WebScreenOrientationTypeFromString(
    const std::string& type) {
  if (type == Emulation::ScreenOrientation::TypeEnum::PortraitPrimary)
    return blink::kWebScreenOrientationPortraitPrimary;
  if (type == Emulation::ScreenOrientation::TypeEnum::PortraitSecondary)
    return blink::kWebScreenOrientationPortraitSecondary;
  if (type == Emulation::ScreenOrientation::TypeEnum::LandscapePrimary)
    return blink::kWebScreenOrientationLandscapePrimary;
  if (type == Emulation::ScreenOrientation::TypeEnum::LandscapeSecondary)
    return blink::kWebScreenOrientationLandscapeSecondary;
  return blink::kWebScreenOrientationUndefined;
}
