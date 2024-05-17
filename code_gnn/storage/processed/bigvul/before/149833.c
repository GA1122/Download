void LayerTreeHost::SetDeviceColorSpace(
    const gfx::ColorSpace& device_color_space) {
  if (device_color_space_ == device_color_space)
    return;
  device_color_space_ = device_color_space;
  LayerTreeHostCommon::CallFunctionForEveryLayer(
       this, [](Layer* layer) { layer->SetNeedsDisplay(); });
 }
