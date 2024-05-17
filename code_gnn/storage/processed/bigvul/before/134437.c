void TabStrip::StartRemoveTabAnimation(int model_index) {
  PrepareForAnimation();

  Tab* tab = tab_at(model_index);
  tab->set_closing(true);

  RemoveTabFromViewModel(model_index);

  ScheduleRemoveTabAnimation(tab);
}
