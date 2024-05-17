void TabStrip::StoppedDraggingTab(Tab* tab, bool* is_first_tab) {
  int tab_data_index = GetModelIndexOfTab(tab);
  if (tab_data_index == -1) {
    return;
  }

  if (*is_first_tab) {
    *is_first_tab = false;
    PrepareForAnimation();

    UpdateIdealBounds();
    AnimateToIdealBounds();
  }

  bounds_animator_.AnimateViewTo(
      tab, ideal_bounds(tab_data_index),
      std::make_unique<ResetDraggingStateDelegate>(this, tab));
}
