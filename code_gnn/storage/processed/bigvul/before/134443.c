void TabStrip::StoppedDraggingTabs(const std::vector<Tab*>& tabs,
                                   const std::vector<int>& initial_positions,
                                   bool move_only,
                                   bool completed) {
  controller()->OnStoppedDraggingTabs();

  newtab_button_->SetVisible(true);
  if (move_only && touch_layout_.get()) {
    if (completed) {
      touch_layout_->SizeToFit();
    } else {
      SetIdealBoundsFromPositions(initial_positions);
    }
  }
  bool is_first_tab = true;
  for (size_t i = 0; i < tabs.size(); ++i)
    StoppedDraggingTab(tabs[i], &is_first_tab);
}
