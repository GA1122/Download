blink::WebWidget* RenderViewTest::GetWebWidget() {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  return impl->GetWebWidget();
}
