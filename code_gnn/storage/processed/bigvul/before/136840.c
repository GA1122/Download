double LocalDOMWindow::scrollY() const {
  if (!GetFrame() || !GetFrame()->GetPage())
    return 0;

  LocalFrameView* view = GetFrame()->View();
  if (!view)
    return 0;

  document()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  double viewport_y =
      view->LayoutViewportScrollableArea()->GetScrollOffset().Height();
  return AdjustForAbsoluteZoom::AdjustScroll(viewport_y,
                                             GetFrame()->PageZoomFactor());
}
