void EmulationHandler::SetDeviceEmulationParams(
    const blink::WebDeviceEmulationParams& params) {
  bool enabled = params != blink::WebDeviceEmulationParams();
  device_emulation_enabled_ = enabled;
  device_emulation_params_ = params;
  UpdateDeviceEmulationState();
}
