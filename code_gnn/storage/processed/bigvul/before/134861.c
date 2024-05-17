float GetDPIScale() {
  if (IsHighDPIEnabled()) {
    return gfx::Display::HasForceDeviceScaleFactor() ?
        gfx::Display::GetForcedDeviceScaleFactor() :
        GetUnforcedDeviceScaleFactor();
  }
   return 1.0;
 }
