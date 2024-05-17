Size DIPToScreenSize(const Size& dip_size) {
  return ToFlooredSize(ScaleSize(dip_size, GetDeviceScaleFactor()));
}
