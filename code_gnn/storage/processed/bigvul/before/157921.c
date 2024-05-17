gfx::RectF RenderViewImpl::ElementBoundsInWindow(
    const blink::WebElement& element) {
  blink::WebRect bounding_box_in_window = element.BoundsInViewport();
  WidgetClient()->ConvertViewportToWindow(&bounding_box_in_window);
  return gfx::RectF(bounding_box_in_window);
}
