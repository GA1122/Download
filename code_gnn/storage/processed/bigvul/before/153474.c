void TabStrip::SetTabVisibility() {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    tab->SetVisible(ShouldTabBeVisible(tab));
  }
  for (const auto& closing_tab : tabs_closing_map_) {
    for (Tab* tab : closing_tab.second)
      tab->SetVisible(ShouldTabBeVisible(tab));
  }
}
