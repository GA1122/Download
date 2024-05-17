void TabStrip::ResizeLayoutTabs() {
  if (tab_count() == 0)
    return;

  RemoveMessageLoopObserver();

  in_tab_close_ = false;
  available_width_for_tabs_ = -1;
  int mini_tab_count = GetMiniTabCount();
  if (mini_tab_count == tab_count()) {
    return;
  }
  StartResizeLayoutAnimation();
}
