void TabStrip::TabTitleChangedNotLoading(int model_index) {
  Tab* tab = tab_at(model_index);
  if (tab->data().mini && !tab->IsActive())
    tab->StartMiniTabTitleAnimation();
}
