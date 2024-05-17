void TabStrip::StartedDraggingTabs(const std::vector<Tab*>& tabs) {
  controller()->OnStartedDraggingTabs();

  if (!drag_controller_.get())
    newtab_button_->SetVisible(false);

  PrepareForAnimation();

  for (int i = 0; i < tab_count(); ++i)
    tab_at(i)->set_dragging(false);

  for (size_t i = 0; i < tabs.size(); ++i) {
    tabs[i]->set_dragging(true);
    bounds_animator_.StopAnimatingView(tabs[i]);
  }

  GenerateIdealBounds();

  for (size_t i = 0; i < tabs.size(); ++i) {
    int tab_data_index = GetModelIndexOfTab(tabs[i]);
    DCHECK_NE(-1, tab_data_index);
    tabs[i]->SetBoundsRect(ideal_bounds(tab_data_index));
  }
  SchedulePaint();
}
