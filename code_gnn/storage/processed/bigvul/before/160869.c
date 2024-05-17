static std::string HitTestAbsoluteUrl(WebView* view, int x, int y) {
  WebPoint hit_point(x, y);
  WebHitTestResult hit_test_result = view->HitTestResultAt(hit_point);
  return hit_test_result.AbsoluteImageURL().GetString().Utf8();
}
