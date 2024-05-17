Response EmulationHandler::ClearDeviceMetricsOverride() {
  if (!device_emulation_enabled_)
    return Response::OK();
  if (GetWebContents())
    GetWebContents()->ClearDeviceEmulationSize();
  else
    return Response::Error("Can't find the associated web contents");
  device_emulation_enabled_ = false;
  device_emulation_params_ = blink::WebDeviceEmulationParams();
  UpdateDeviceEmulationState();
  return Response::FallThrough();
}
