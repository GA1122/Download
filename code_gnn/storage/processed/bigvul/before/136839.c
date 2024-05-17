double LocalDOMWindow::scrollX() const {
  if (!GetFrame() || !GetFrame()->GetPage())
    return 0;

  LocalFrameView* view = GetFrame()->View();
  if (!view)
    return 0;

  document()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  double viewport_x =
      view->LayoutViewportScrollableArea()->GetScrollOffset().Width();
  return AdjustForAbsoluteZoom::AdjustScroll(viewport_x,
                                             GetFrame()->PageZoomFactor());
}
