void PaintLayerScrollableArea::SnapAfterScrollbarScrolling(
    ScrollbarOrientation orientation) {
  SnapCoordinator* snap_coordinator =
      GetLayoutBox()->GetDocument().GetSnapCoordinator();
  if (!snap_coordinator)
    return;

  snap_coordinator->SnapAtCurrentPosition(*GetLayoutBox(),
                                          orientation == kHorizontalScrollbar,
                                          orientation == kVerticalScrollbar);
}
