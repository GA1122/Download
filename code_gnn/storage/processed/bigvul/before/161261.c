void EmulationHandler::UpdateTouchEventEmulationState() {
  RenderWidgetHostImpl* widget_host =
      host_ ? host_->GetRenderWidgetHost() : nullptr;
  if (!widget_host)
    return;
  if (touch_emulation_enabled_) {
    widget_host->GetTouchEmulator()->Enable(
        TouchEmulator::Mode::kEmulatingTouchFromMouse,
        TouchEmulationConfigurationToType(touch_emulation_configuration_));
  } else {
    widget_host->GetTouchEmulator()->Disable();
  }
  if (GetWebContents()) {
    GetWebContents()->SetForceDisableOverscrollContent(
        touch_emulation_enabled_);
  }
}
