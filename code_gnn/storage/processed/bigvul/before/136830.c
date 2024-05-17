void LocalDOMWindow::resizeBy(int x, int y) const {
  if (!GetFrame() || !GetFrame()->IsMainFrame())
    return;

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  IntRect fr = page->GetChromeClient().RootWindowRect();
  IntSize dest = fr.Size() + IntSize(x, y);
  IntRect update(fr.Location(), dest);
  page->GetChromeClient().SetWindowRectWithAdjustment(update, *GetFrame());
}
