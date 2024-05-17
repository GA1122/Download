int BrowserView::NonClientHitTest(const gfx::Point& point) {
  return GetBrowserViewLayout()->NonClientHitTest(point);
}
