void TabStrip::PrepareForCloseAt(int model_index, CloseTabSource source) {
  if (!in_tab_close_ && IsAnimating()) {
    StopAnimating(true);
  }

  if (!GetWidget())
    return;

  int model_count = GetModelCount();
  if (model_index + 1 != model_count && model_count > 1) {
    Tab* last_tab = tab_at(model_count - 1);
    Tab* tab_being_removed = tab_at(model_index);
    available_width_for_tabs_ = last_tab->x() + last_tab->width() -
        tab_being_removed->width() - tab_h_offset();
    if (model_index == 0 && tab_being_removed->data().mini &&
        !tab_at(1)->data().mini) {
      available_width_for_tabs_ -= kMiniToNonMiniGap;
    }
  }

  in_tab_close_ = true;
  resize_layout_timer_.Stop();
  if (source == CLOSE_TAB_FROM_TOUCH) {
    StartResizeLayoutTabsFromTouchTimer();
  } else {
    AddMessageLoopObserver();
  }
}
