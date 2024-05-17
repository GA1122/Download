void InitDeviceScaleFactor(float scale) {
  DCHECK_NE(0.0f, scale);
  g_device_scale_factor = scale;
}
