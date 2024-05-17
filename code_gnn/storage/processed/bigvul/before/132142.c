void RenderViewTest::DidNavigateWithinPage(blink::WebLocalFrame* frame,
                                           bool is_new_navigation) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  blink::WebHistoryItem item;
  item.initialize();
  impl->GetMainRenderFrame()->didNavigateWithinPage(
      frame,
      item,
      is_new_navigation ? blink::WebStandardCommit
                        : blink::WebHistoryInertCommit);
}
