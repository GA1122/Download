Size ScreenToDIPSize(const Size& size_in_pixels) {
  return ToFlooredSize(
      ScaleSize(size_in_pixels, 1.0f / GetDeviceScaleFactor()));
}
