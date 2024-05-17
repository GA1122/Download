float GetDeviceScaleFactor() {
  DCHECK_NE(0.0f, g_device_scale_factor);
  return g_device_scale_factor;
}
