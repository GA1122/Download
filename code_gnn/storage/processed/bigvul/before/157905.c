void RenderViewImpl::ConvertViewportToWindowViaWidget(blink::WebRect* rect) {
  WidgetClient()->ConvertViewportToWindow(rect);
}
