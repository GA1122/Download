void RenderViewTest::SetFocused(const WebKit::WebNode& node) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->focusedNodeChanged(node);
}
