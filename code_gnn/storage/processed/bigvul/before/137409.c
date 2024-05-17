void RenderViewTest::SetFocused(const blink::WebNode& node) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->FocusedNodeChanged(blink::WebNode(), node);
}
