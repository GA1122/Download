Rect ScreenToDIPRect(const Rect& pixel_bounds) {
  return ToFlooredRectDeprecated(
      ScaleRect(pixel_bounds, 1.0f / GetDeviceScaleFactor()));
}
