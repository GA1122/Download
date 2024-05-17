float GetUnforcedDeviceScaleFactor() {
  return static_cast<float>(gfx::GetDPI().width()) /
      static_cast<float>(kDefaultDPIX);
}
