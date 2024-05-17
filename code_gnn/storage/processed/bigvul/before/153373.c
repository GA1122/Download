void TabStrip::CloseTab(Tab* tab, CloseTabSource source) {
  int model_index = GetModelIndexOfTab(tab);
  if (tab->closing()) {
    model_index = FindClosingTab(tab).first->first;
  }

  if (!IsValidModelIndex(model_index))
    return;

  if (!controller_->BeforeCloseTab(model_index, source))
    return;

  if (!in_tab_close_ && IsAnimating()) {
    StopAnimating(true);
  }

  if (GetWidget()) {
    in_tab_close_ = true;
    resize_layout_timer_.Stop();
    if (source == CLOSE_TAB_FROM_TOUCH)
      StartResizeLayoutTabsFromTouchTimer();
    else
      AddMessageLoopObserver();
  }

  UpdateHoverCard(nullptr, false);
  controller_->CloseTab(model_index, source);
}
