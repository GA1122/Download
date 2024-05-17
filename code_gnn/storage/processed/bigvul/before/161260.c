void EmulationHandler::UpdateDeviceEmulationState() {
  RenderWidgetHostImpl* widget_host =
      host_ ? host_->GetRenderWidgetHost() : nullptr;
  if (!widget_host)
    return;
  if (device_emulation_enabled_) {
    widget_host->Send(new ViewMsg_EnableDeviceEmulation(
        widget_host->GetRoutingID(), device_emulation_params_));
  } else {
    widget_host->Send(new ViewMsg_DisableDeviceEmulation(
        widget_host->GetRoutingID()));
  }
}
