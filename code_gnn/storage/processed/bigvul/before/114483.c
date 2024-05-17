void RenderingHelper::PlatformInitialize() {
  CHECK(x_display_ = base::MessagePumpForUI::GetDefaultXDisplay());
}
