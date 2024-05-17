int GpuChannel::GenerateRouteID() {
  static int last_id = 0;
  return ++last_id;
}
