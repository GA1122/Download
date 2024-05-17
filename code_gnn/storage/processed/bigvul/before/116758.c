void RenderViewTest::DidNavigateWithinPage(WebKit::WebFrame* frame,
                                           bool is_new_navigation) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  impl->didNavigateWithinPage(frame, is_new_navigation);
}
