void RenderWidgetHostViewPort::GetDefaultScreenInfo(
    WebKit::WebScreenInfo* results) {
  DeviceInfo info;
  const int width = info.GetWidth();
  const int height = info.GetHeight();
  results->deviceScaleFactor = info.GetDPIScale();
  results->depth = info.GetBitsPerPixel();
  results->depthPerComponent = info.GetBitsPerComponent();
  results->isMonochrome = (results->depthPerComponent == 0);
  results->rect = WebKit::WebRect(0, 0, width, height);
  results->availableRect = WebKit::WebRect(0, 0, width, height);
}
