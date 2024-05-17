void LocalDOMWindow::moveTo(int x, int y) const {
  if (!GetFrame() || !GetFrame()->IsMainFrame())
    return;

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  IntRect window_rect = page->GetChromeClient().RootWindowRect();
  window_rect.SetLocation(IntPoint(x, y));
  page->GetChromeClient().SetWindowRectWithAdjustment(window_rect, *GetFrame());
}
