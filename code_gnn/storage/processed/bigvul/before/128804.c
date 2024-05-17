EGLNativeDisplayType GetPlatformDefaultEGLNativeDisplay() {
  return ui::OzonePlatform::GetInstance()
      ->GetSurfaceFactoryOzone()
      ->GetNativeDisplay();
}
