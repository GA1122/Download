void TabStripGtk::HandleGlobalMouseMoveEvent() {
  if (!IsCursorInTabStripZone()) {
    if (!weak_factory_.HasWeakPtrs()) {
      MessageLoop::current()->PostDelayedTask(
          FROM_HERE,
          base::Bind(&TabStripGtk::ResizeLayoutTabs,
                     weak_factory_.GetWeakPtr()),
          base::TimeDelta::FromMilliseconds(kResizeTabsTimeMs));
    }
  } else {
    weak_factory_.InvalidateWeakPtrs();
  }
}
