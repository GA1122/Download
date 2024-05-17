void TabStrip::RemoveTabAt(int model_index) {
  if (touch_layout_.get()) {
    Tab* tab = tab_at(model_index);
    tab->set_closing(true);
    int old_x = tabs_.ideal_bounds(model_index).x();
    RemoveTabFromViewModel(model_index);
    touch_layout_->RemoveTab(model_index, GenerateIdealBoundsForMiniTabs(NULL),
                             old_x);
    ScheduleRemoveTabAnimation(tab);
  } else if (in_tab_close_ && model_index != GetModelCount()) {
    StartMouseInitiatedRemoveTabAnimation(model_index);
  } else {
    StartRemoveTabAnimation(model_index);
  }
  SwapLayoutIfNecessary();

  FOR_EACH_OBSERVER(TabStripObserver, observers_,
                    TabStripRemovedTabAt(this, model_index));
}
