Response EmulationHandler::Disable() {
  if (touch_emulation_enabled_) {
    touch_emulation_enabled_ = false;
    UpdateTouchEventEmulationState();
  }
  device_emulation_enabled_ = false;
  UpdateDeviceEmulationState();
  return Response::OK();
}
