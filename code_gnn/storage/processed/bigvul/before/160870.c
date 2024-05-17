static std::string HitTestElementId(WebView* view, int x, int y) {
  WebPoint hit_point(x, y);
  WebHitTestResult hit_test_result = view->HitTestResultAt(hit_point);
  return hit_test_result.GetNode().To<WebElement>().GetAttribute("id").Utf8();
}
