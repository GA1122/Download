void WebLocalFrameImpl::FindMatchRects(WebVector<WebFloatRect>& output_rects) {
  EnsureTextFinder().FindMatchRects(output_rects);
}
