Tab* TabStrip::CreateTabForDragging() {
  Tab* tab = new Tab(NULL);
  tab->set_theme_provider(GetThemeProvider());
  return tab;
}
