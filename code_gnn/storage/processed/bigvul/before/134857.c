Point DIPToScreenPoint(const Point& dip_point) {
  return ToFlooredPoint(ScalePoint(dip_point, GetDeviceScaleFactor()));
}
