void TabStrip::ResizeLayoutTabsFromTouch() {
  if (!drag_context_->IsDragSessionActive())
    ResizeLayoutTabs();
  else
    StartResizeLayoutTabsFromTouchTimer();
}
