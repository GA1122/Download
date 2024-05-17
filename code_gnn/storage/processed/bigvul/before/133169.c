void HWNDMessageHandler::SetRegion(HRGN region) {
  custom_window_region_.Set(region);
  ResetWindowRegion(false, true);
}
