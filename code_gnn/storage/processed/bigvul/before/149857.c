void LayerTreeHost::SetPaintedDeviceScaleFactor(
    float painted_device_scale_factor) {
  if (painted_device_scale_factor_ == painted_device_scale_factor)
    return;
  painted_device_scale_factor_ = painted_device_scale_factor;

  SetNeedsCommit();
}
