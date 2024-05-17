bool TouchEventConverterEvdev::Reinitialize() {
  EventDeviceInfo info;
  if (info.Initialize(fd_)) {
    Initialize(info);
    return true;
  }
  return false;
}
