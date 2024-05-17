MeteringMode ParseMeteringMode(const String& blink_mode) {
  if (blink_mode == "manual")
    return MeteringMode::MANUAL;
  if (blink_mode == "single-shot")
    return MeteringMode::SINGLE_SHOT;
  if (blink_mode == "continuous")
    return MeteringMode::CONTINUOUS;
  if (blink_mode == "none")
    return MeteringMode::NONE;
  NOTREACHED();
  return MeteringMode::NONE;
}
