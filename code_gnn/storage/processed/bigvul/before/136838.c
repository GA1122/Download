void LocalDOMWindow::scrollTo(const ScrollToOptions& scroll_to_options) const {
  if (!IsCurrentlyDisplayedInFrame())
    return;

  LocalFrameView* view = GetFrame()->View();
  if (!view)
    return;

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  if (GetFrame()->Loader().GetDocumentLoader()) {
    GetFrame()
        ->Loader()
        .GetDocumentLoader()
        ->GetInitialScrollState()
        .was_scrolled_by_js = true;
  }

  if (!scroll_to_options.hasLeft() || !scroll_to_options.hasTop() ||
      scroll_to_options.left() || scroll_to_options.top()) {
    document()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  }

  double scaled_x = 0.0;
  double scaled_y = 0.0;

  ScrollableArea* viewport = view->LayoutViewportScrollableArea();
  ScrollOffset current_offset = viewport->GetScrollOffset();
  scaled_x = current_offset.Width();
  scaled_y = current_offset.Height();

  if (scroll_to_options.hasLeft())
    scaled_x =
        ScrollableArea::NormalizeNonFiniteScroll(scroll_to_options.left()) *
        GetFrame()->PageZoomFactor();

  if (scroll_to_options.hasTop())
    scaled_y =
        ScrollableArea::NormalizeNonFiniteScroll(scroll_to_options.top()) *
        GetFrame()->PageZoomFactor();

  FloatPoint new_scaled_position = ScrollOffsetToPosition(
      ScrollOffset(scaled_x, scaled_y), viewport->ScrollOrigin());
  if (SnapCoordinator* coordinator = document()->GetSnapCoordinator()) {
    new_scaled_position = coordinator->GetSnapPositionForPoint(
        *document()->GetLayoutView(), new_scaled_position,
        scroll_to_options.hasLeft(), scroll_to_options.hasTop());
  }

  ScrollBehavior scroll_behavior = kScrollBehaviorAuto;
  ScrollableArea::ScrollBehaviorFromString(scroll_to_options.behavior(),
                                           scroll_behavior);

  viewport->SetScrollOffset(
      ScrollPositionToOffset(new_scaled_position, viewport->ScrollOrigin()),
      kProgrammaticScroll, scroll_behavior);
}
