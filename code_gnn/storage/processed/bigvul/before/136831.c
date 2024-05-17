void LocalDOMWindow::resizeTo(int width, int height) const {
  if (!GetFrame() || !GetFrame()->IsMainFrame())
    return;

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  IntRect fr = page->GetChromeClient().RootWindowRect();
  IntSize dest = IntSize(width, height);
  IntRect update(fr.Location(), dest);
  page->GetChromeClient().SetWindowRectWithAdjustment(update, *GetFrame());
}
