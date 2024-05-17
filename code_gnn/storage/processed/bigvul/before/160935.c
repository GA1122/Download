 bool ChromeClientImpl::CanTakeFocus(WebFocusType) {
  return !LayoutTestSupport::IsRunningLayoutTest();
}
