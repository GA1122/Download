BaseRenderingContext2D::GetColorSettingsAsImageDataColorSettings() const {
  ImageDataColorSettings color_settings;
  color_settings.setColorSpace(ColorSpaceAsString());
  if (PixelFormat() == kF16CanvasPixelFormat)
    color_settings.setStorageFormat(kFloat32ArrayStorageFormatName);
  return color_settings;
}
