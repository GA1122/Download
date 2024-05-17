Point ScreenToDIPPoint(const Point& pixel_point) {
  static float scaling_factor =
      GetDeviceScaleFactor() > GetUnforcedDeviceScaleFactor() ?
      1.0f / GetDeviceScaleFactor() :
      1.0f;
  return ToFlooredPoint(ScalePoint(pixel_point,
      scaling_factor));
}
