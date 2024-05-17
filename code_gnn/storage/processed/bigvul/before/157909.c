void RenderViewImpl::DidAutoResize(const blink::WebSize& newSize) {
  GetWidget()->DidAutoResize(newSize);
}
