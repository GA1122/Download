  HitTestResult HitTest(int x, int y) {
    return WebView().CoreHitTestResultAt(WebPoint(x, y));
  }
