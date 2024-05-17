void TabStrip::ResizeLayoutTabsFromTouch() {
  if (!drag_controller_.get())
    ResizeLayoutTabs();
  else
    StartResizeLayoutTabsFromTouchTimer();
}
